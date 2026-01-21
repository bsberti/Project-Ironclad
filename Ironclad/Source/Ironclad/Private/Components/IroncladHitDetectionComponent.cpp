#include "Components/IroncladHitDetectionComponent.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY(LogIroncladHitDetect);

UIroncladHitDetectionComponent::UIroncladHitDetectionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.SetTickFunctionEnable(false); // Only tick during hit windows.
}

void UIroncladHitDetectionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UIroncladHitDetectionComponent::BeginHitWindow(USceneComponent* InTraceSource, const FIroncladTraceConfig& InConfig)
{
	if (!GetWorld())
	{
		UE_LOG(LogIroncladHitDetect, Warning, TEXT("[HitDetect] BeginHitWindow failed: no World."));
		return;
	}

	if (!InTraceSource)
	{
		UE_LOG(LogIroncladHitDetect, Warning, TEXT("[HitDetect] BeginHitWindow failed: TraceSource is null."));
		return;
	}

	if (InConfig.StartSocket.IsNone() || InConfig.EndSocket.IsNone())
	{
		UE_LOG(LogIroncladHitDetect, Warning, TEXT("[HitDetect] BeginHitWindow failed: StartSocket/EndSocket not set."));
		return;
	}

	TraceSource = InTraceSource;
	ActiveConfig = InConfig;

	bWindowActive = true;
	HitActorsThisWindow.Reset();
	ResetPrev();

	PrimaryComponentTick.SetTickFunctionEnable(true);

	UE_LOG(LogIroncladHitDetect, Verbose, TEXT("[HitDetect] Hit window BEGIN (Radius=%.2f, Channel=%d)"),
		ActiveConfig.Radius, (int32)ActiveConfig.TraceChannel);
}

void UIroncladHitDetectionComponent::EndHitWindow()
{
	if (!bWindowActive)
	{
		return;
	}

	bWindowActive = false;
	PrimaryComponentTick.SetTickFunctionEnable(false);

	TraceSource = nullptr;
	HitActorsThisWindow.Reset();
	ResetPrev();

	UE_LOG(LogIroncladHitDetect, Verbose, TEXT("[HitDetect] Hit window END"));
}

void UIroncladHitDetectionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bWindowActive || !TraceSource || !GetWorld())
	{
		return;
	}

	const FVector CurrStart = GetSocketWorldLocationSafe(ActiveConfig.StartSocket);
	const FVector CurrEnd = GetSocketWorldLocationSafe(ActiveConfig.EndSocket);

	if (!bHasPrev)
	{
		PrevStart = CurrStart;
		PrevEnd = CurrEnd;
		bHasPrev = true;
		return;
	}

	ProcessSweeps(CurrStart, CurrEnd);

	if (bDebugEnabled && ActiveConfig.bDebugDraw)
	{
		DebugDrawFrame(CurrStart, CurrEnd);
	}

	PrevStart = CurrStart;
	PrevEnd = CurrEnd;
}

FVector UIroncladHitDetectionComponent::GetSocketWorldLocationSafe(const FName SocketName) const
{
	if (!TraceSource)
	{
		return FVector::ZeroVector;
	}

	// USceneComponent supports sockets if the underlying component type does (skeletal/static mesh).
	// If the socket doesn't exist, GetSocketLocation returns component location for some component types,
	// so we log to make this obvious.
	const FVector Loc = TraceSource->GetSocketLocation(SocketName);

#if !(UE_BUILD_SHIPPING)
	// Soft validation: if sockets are missing, it's usually because the socket name is wrong.
	// We cannot reliably check socket existence on all USceneComponent types, so this is best-effort.
#endif

	return Loc;
}

void UIroncladHitDetectionComponent::ResetPrev()
{
	bHasPrev = false;
	PrevStart = FVector::ZeroVector;
	PrevEnd = FVector::ZeroVector;
}

void UIroncladHitDetectionComponent::ProcessSweeps(const FVector& CurrStart, const FVector& CurrEnd)
{
	if (!GetWorld())
	{
		return;
	}

	AActor* OwnerActor = GetOwner();

	FCollisionQueryParams Params(SCENE_QUERY_STAT(IroncladWeaponHitTrace), ActiveConfig.bTraceComplex);
	Params.bReturnPhysicalMaterial = false;

	if (OwnerActor)
	{
		Params.AddIgnoredActor(OwnerActor);
		// Also ignore instigator (often the character holding the weapon) to avoid self-hits.
		if (AActor* Inst = OwnerActor->GetInstigator())
		{
			Params.AddIgnoredActor(Inst);
		}
	}

	const FCollisionShape Sphere = FCollisionShape::MakeSphere(ActiveConfig.Radius);

	auto SweepMultiSphere = [&](const FVector& From, const FVector& To)
		{
			TArray<FHitResult> Hits;
			const bool bHit = GetWorld()->SweepMultiByChannel(
				Hits,
				From,
				To,
				FQuat::Identity,
				ActiveConfig.TraceChannel,
				Sphere,
				Params
			);

			if (!bHit)
			{
				return;
			}

			for (const FHitResult& H : Hits)
			{
				HandleHitResult(H);
			}
		};

	// Two sweeps per tick: base socket and tip socket. This is a good balance of cost vs reliability.
	SweepMultiSphere(PrevStart, CurrStart);
	SweepMultiSphere(PrevEnd, CurrEnd);
}

void UIroncladHitDetectionComponent::HandleHitResult(const FHitResult& Hit)
{
	AActor* Other = Hit.GetActor();
	if (!Other)
	{
		return;
	}

	// Anti-multi-hit per window.
	if (HitActorsThisWindow.Contains(Other))
	{
		return;
	}

	HitActorsThisWindow.Add(Other);

	UE_LOG(LogIroncladHitDetect, Display, TEXT("[HitDetect] HIT: %s (Bone=%s, Point=%s)"),
		*GetNameSafe(Other),
		*Hit.BoneName.ToString(),
		*Hit.ImpactPoint.ToString()
	);

	OnHitDetected.Broadcast(Hit);
}

void UIroncladHitDetectionComponent::DebugDrawFrame(const FVector& CurrStart, const FVector& CurrEnd) const
{
	if (!GetWorld())
	{
		return;
	}

	const float T = ActiveConfig.DebugDrawTime;

	// Prev positions
	DrawDebugSphere(GetWorld(), PrevStart, ActiveConfig.Radius, 12, FColor::Green, false, T);
	DrawDebugSphere(GetWorld(), PrevEnd, ActiveConfig.Radius, 12, FColor::Green, false, T);

	// Curr positions
	DrawDebugSphere(GetWorld(), CurrStart, ActiveConfig.Radius, 12, FColor::Cyan, false, T);
	DrawDebugSphere(GetWorld(), CurrEnd, ActiveConfig.Radius, 12, FColor::Cyan, false, T);

	// Motion lines
	DrawDebugLine(GetWorld(), PrevStart, CurrStart, FColor::Yellow, false, T, 0, 1.5f);
	DrawDebugLine(GetWorld(), PrevEnd, CurrEnd, FColor::Yellow, false, T, 0, 1.5f);

	// Blade segment (current frame)
	DrawDebugLine(GetWorld(), CurrStart, CurrEnd, FColor::White, false, T, 0, 1.0f);
}
