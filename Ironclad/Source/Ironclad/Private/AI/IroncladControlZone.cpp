#include "AI/IroncladControlZone.h"

#include "DrawDebugHelpers.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"

AIroncladControlZone::AIroncladControlZone()
{
	PrimaryActorTick.bCanEverTick = false;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);

	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionObjectType(ECC_WorldDynamic);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AIroncladControlZone::OnSphereBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AIroncladControlZone::OnSphereEndOverlap);

	// Note: Radius is applied in BeginPlay (so BP edits are respected)
}

void AIroncladControlZone::BeginPlay()
{
	Super::BeginPlay();

	Sphere->SetSphereRadius(Radius);
	SetLifeSpan(DurationSeconds);

	// Debug visualization (only in non-shipping builds)
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	DrawDebugSphere(
		GetWorld(),
		GetActorLocation(),
		Radius,
		24,
		FColor::Cyan,
		false,
		DurationSeconds,
		0,
		2.0f
	);
#endif
}

void AIroncladControlZone::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RestoreAll();
	Super::EndPlay(EndPlayReason);
}

void AIroncladControlZone::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("[Zone] BeginOverlap: %s"), *GetNameSafe(OtherActor));

	ACharacter* Char = Cast<ACharacter>(OtherActor);
	if (!Char) return;

	if (bAffectPlayersOnly)
	{
		APawn* Pawn = Cast<APawn>(Char);
		if (!Pawn || !Pawn->IsPlayerControlled())
		{
			return;
		}
	}

	ApplySlow(Char);
}

void AIroncladControlZone::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* Char = Cast<ACharacter>(OtherActor);
	if (!Char) return;

	RemoveSlow(Char);
}

void AIroncladControlZone::ApplySlow(ACharacter* Char)
{
	if (!Char) return;

	UCharacterMovementComponent* Move = Char->GetCharacterMovement();
	if (!Move) return;

	FSpeedState& State = Affected.FindOrAdd(Char);

	UE_LOG(LogTemp, Warning, TEXT("[Zone] Slow applied: %s Speed %.1f -> %.1f"),
		*GetNameSafe(Char),
		State.OriginalSpeed,
		Move->MaxWalkSpeed);

	// First time we affect this character, capture original.
	if (State.StackCount == 0)
	{
		State.OriginalSpeed = Move->MaxWalkSpeed;
		Move->MaxWalkSpeed = State.OriginalSpeed * SlowMultiplier;
	}

	State.StackCount++;
}

void AIroncladControlZone::RemoveSlow(ACharacter* Char)
{
	if (!Char) return;

	FSpeedState* State = Affected.Find(Char);
	if (!State) return;

	State->StackCount = FMath::Max(0, State->StackCount - 1);

	// Only restore when last zone releases them.
	if (State->StackCount == 0)
	{
		if (UCharacterMovementComponent* Move = Char->GetCharacterMovement())
		{
			Move->MaxWalkSpeed = State->OriginalSpeed;
			UE_LOG(LogTemp, Warning, TEXT("[Zone] Slow removed: %s Speed -> %.1f"),
				*GetNameSafe(Char),
				Move->MaxWalkSpeed);
		}
		Affected.Remove(Char);
	}
}

void AIroncladControlZone::RestoreAll()
{
	for (auto& Pair : Affected)
	{
		ACharacter* Char = Pair.Key.Get();
		if (!Char) continue;

		if (UCharacterMovementComponent* Move = Char->GetCharacterMovement())
		{
			Move->MaxWalkSpeed = Pair.Value.OriginalSpeed;
		}
	}
	Affected.Empty();
}
