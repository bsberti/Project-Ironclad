#include "AI/IroncladEnemyAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/IroncladPlayerCharacter.h"
#include "Characters/IroncladControllerEnemy.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISense_Sight.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

// Global log category (one definition in .cpp)
DEFINE_LOG_CATEGORY(LogIroncladPerception);

// Global toggle CVar (0/1)
static TAutoConsoleVariable<int32> CVarIroncladAIDebug(
	TEXT("ironclad.ai.debug"),
	0,
	TEXT("Enable Ironclad AI debug drawing (0=off, 1=on)."),
	ECVF_Cheat
);

static const FName BBKey_EnemyArchetype(TEXT("EnemyArchetype"));

static FString IC_PercPrefix(const AAIController* Controller)
{
	const APawn* Pawn = Controller ? Controller->GetPawn() : nullptr;
	const FString PawnName = Pawn ? Pawn->GetName() : TEXT("NoPawn");
	const FString CtrlName = Controller ? Controller->GetName() : TEXT("NoController");
	return FString::Printf(TEXT("[Perception][Ctrl=%s][Pawn=%s]"), *CtrlName, *PawnName);
}

AIroncladEnemyAIController::AIroncladEnemyAIController()
{
	// Only tick when debug is enabled (we'll enable/disable dynamically in Tick)
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(true);

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	// --- Sight ---
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 2000.0f;
	SightConfig->LoseSightRadius = 2400.0f;
	SightConfig->PeripheralVisionAngleDegrees = 110.0f;
	SightConfig->SetMaxAge(2.0f);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(UAISense_Sight::StaticClass());

	// --- Hearing ---
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 1500.0f;
	HearingConfig->LoSHearingRange = 1500.0f;
	HearingConfig->SetMaxAge(2.0f);

	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerception->ConfigureSense(*HearingConfig);

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
}

void AIroncladEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogIroncladPerception, Log, TEXT("%s Ready Dominant=%s"),
		*IC_PercPrefix(this),
		*GetNameSafe(GetPerceptionComponent() ? GetPerceptionComponent()->GetDominantSense() : nullptr));

	if (ensure(AIPerception))
	{
		AIPerception->OnPerceptionUpdated.AddDynamic(this, &AIroncladEnemyAIController::OnPerceptionUpdated);
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AIroncladEnemyAIController::OnTargetPerceptionUpdated);
	}
}

void AIroncladEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Global toggle
	const bool bDebug = (CVarIroncladAIDebug.GetValueOnGameThread() != 0);

	// If disabled, do nothing. (Tick still runs, but it's minimal.)
	if (!bDebug)
	{
		return;
	}

	DrawAIDebug();
}

bool AIroncladEnemyAIController::BlackboardHasKey(const FName& KeyName) const
{
	if (!BlackboardComp) return false;
	// GetKeyID returns FBlackboard::InvalidKey if missing
	return BlackboardComp->GetKeyID(KeyName) != FBlackboard::InvalidKey;
}

FString AIroncladEnemyAIController::BuildDebugText() const
{
	const APawn* SelfPawn = GetPawn();
	const FVector SelfLoc = SelfPawn ? SelfPawn->GetActorLocation() : FVector::ZeroVector;

	AActor* Target = nullptr;
	bool bHasTarget = false;

	if (BlackboardComp && BlackboardHasKey(TEXT("TargetActor")))
	{
		Target = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));
	}
	if (BlackboardComp && BlackboardHasKey(TEXT("HasTarget")))
	{
		bHasTarget = BlackboardComp->GetValueAsBool(TEXT("HasTarget"));
	}

	const float AttackRange = (BlackboardComp && BlackboardHasKey(TEXT("AttackRange")))
		? BlackboardComp->GetValueAsFloat(TEXT("AttackRange"))
		: 0.0f;

	const float DistToTarget = (Target)
		? FVector::Dist(SelfLoc, Target->GetActorLocation())
		: -1.0f;

	// Perception status (sight/hearing currently sensing target?)
	bool bPerceivingTarget = false;
	float LastStimAge = -1.0f;
	FString SenseSummary = TEXT("None");

	if (AIPerception && Target)
	{
		FActorPerceptionBlueprintInfo Info;
		AIPerception->GetActorsPerception(Target, Info);

		for (const FAIStimulus& S : Info.LastSensedStimuli)
		{
			if (S.WasSuccessfullySensed())
			{
				bPerceivingTarget = true;
				LastStimAge = S.GetAge();
				SenseSummary = S.Type.IsValid() ? S.Type.Name.ToString() : TEXT("Sense");
				break;
			}
		}
	}

	// Optional keys (only if they exist in your BB)
	const bool bShouldZone = (BlackboardComp && BlackboardHasKey(TEXT("ShouldPlaceZone")))
		? BlackboardComp->GetValueAsBool(TEXT("ShouldPlaceZone"))
		: false;

	const FVector HomeLoc = (BlackboardComp && BlackboardHasKey(TEXT("HomeLocation")))
		? BlackboardComp->GetValueAsVector(TEXT("HomeLocation"))
		: FVector::ZeroVector;

	const float DistToHome = (SelfPawn && BlackboardComp && BlackboardHasKey(TEXT("HomeLocation")))
		? FVector::Dist(SelfLoc, HomeLoc)
		: -1.0f;

	const FString TargetName = Target ? Target->GetName() : TEXT("None");

	// If you later add an explicit “AIState” key (enum or name), it can go here safely
	const FString StateLine = TEXT("State: (add BB key if desired)");

	return FString::Printf(
		TEXT("%s\nHasTarget: %s\nTarget: %s (Dist: %.0f)\nPerception: %s (Active: %s, Age: %.2f)\nAttackRange: %.0f\nShouldZone: %s\nDistToHome: %.0f"),
		*StateLine,
		bHasTarget ? TEXT("true") : TEXT("false"),
		*TargetName,
		DistToTarget,
		*SenseSummary,
		bPerceivingTarget ? TEXT("true") : TEXT("false"),
		LastStimAge,
		AttackRange,
		bShouldZone ? TEXT("true") : TEXT("false"),
		DistToHome
	);
}

void AIroncladEnemyAIController::DrawAIDebug() const
{
	UWorld* World = GetWorld();
	const APawn* SelfPawn = GetPawn();
	if (!World || !SelfPawn) return;

	const FVector SelfLoc = SelfPawn->GetActorLocation();

	// --- Text above head ---
	const FVector TextLoc = SelfLoc + FVector(0, 0, 120.0f);
	const FString Text = BuildDebugText();
	DrawDebugString(World, TextLoc, Text, nullptr, FColor::White, 0.0f, true);

	// --- Perception radii ---
	if (SightConfig)
	{
		DrawDebugSphere(World, SelfLoc, SightConfig->SightRadius, 24, FColor::Green, false, 0.0f, 0, 1.0f);
		DrawDebugSphere(World, SelfLoc, SightConfig->LoseSightRadius, 24, FColor::Yellow, false, 0.0f, 0, 1.0f);
	}

	// --- Attack range ---
	if (BlackboardComp && BlackboardHasKey(TEXT("AttackRange")))
	{
		const float AttackRange = BlackboardComp->GetValueAsFloat(TEXT("AttackRange"));
		if (AttackRange > 0.0f)
		{
			DrawDebugSphere(World, SelfLoc, AttackRange, 18, FColor::Red, false, 0.0f, 0, 1.5f);
		}
	}

	// --- Target line & last known location ---
	AActor* Target = nullptr;
	if (BlackboardComp && BlackboardHasKey(TEXT("TargetActor")))
	{
		Target = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));
	}
	if (Target)
	{
		const FVector TargetLoc = Target->GetActorLocation();
		DrawDebugLine(World, SelfLoc + FVector(0, 0, 50), TargetLoc + FVector(0, 0, 50), FColor::Cyan, false, 0.0f, 0, 1.5f);
	}

	if (BlackboardComp && BlackboardHasKey(TEXT("LastKnownLocation")))
	{
		const FVector LKL = BlackboardComp->GetValueAsVector(TEXT("LastKnownLocation"));
		DrawDebugSphere(World, LKL, 30.0f, 12, FColor::Cyan, false, 0.0f, 0, 2.0f);
	}

	// --- Facing arrow (helps tuning turn/rotation) ---
	const FVector Fwd = SelfPawn->GetActorForwardVector();
	DrawDebugDirectionalArrow(World, SelfLoc + FVector(0, 0, 30), SelfLoc + FVector(0, 0, 30) + Fwd * 150.0f, 50.0f, FColor::White, false, 0.0f, 0, 2.0f);
}

void AIroncladEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	UE_LOG(LogIroncladPerception, Display, TEXT("%s UpdatedActors=%d"),
		*IC_PercPrefix(this), UpdatedActors.Num());

	if (!BlackboardComp || !GetPerceptionComponent())
	{
		return;
	}

	for (AActor* Actor : UpdatedActors)
	{
		if (!IsValid(Actor)) continue;
		if (!Actor->IsA(AIroncladPlayerCharacter::StaticClass())) continue;

		FActorPerceptionBlueprintInfo Info;
		GetPerceptionComponent()->GetActorsPerception(Actor, Info);

		const bool bSensedNow = Info.LastSensedStimuli.ContainsByPredicate(
			[](const FAIStimulus& S) { return S.WasSuccessfullySensed(); }
		);

		if (bSensedNow)
		{
			BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Actor);
			BlackboardComp->SetValueAsBool(TEXT("HasTarget"), true);
			BlackboardComp->SetValueAsVector(TEXT("LastKnownLocation"), Actor->GetActorLocation());

			// IMPORTANT: do NOT set SearchOrigin here. Only set it when starting search.
			continue;
		}

		// Lost stimulus case: only react if this actor is the current BB target
		AActor* CurrentBBTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));
		if (Actor != CurrentBBTarget)
		{
			continue;
		}

		// Clear target immediately
		BlackboardComp->ClearValue(TEXT("TargetActor"));
		BlackboardComp->SetValueAsBool(TEXT("HasTarget"), false);

		// If already searching, don't reinitialize (prevents endless search)
		if (BlackboardComp->GetValueAsBool(TEXT("ShouldSearch")))
		{
			continue;
		}

		const float Now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;

		// Establish SearchOrigin from LastKnownLocation (fallback chain)
		FVector Origin = BlackboardComp->GetValueAsVector(TEXT("LastKnownLocation"));

		if (Origin.IsNearlyZero() && IsValid(Actor))
		{
			Origin = Actor->GetActorLocation();
			BlackboardComp->SetValueAsVector(TEXT("LastKnownLocation"), Origin);
		}

		if (Origin.IsNearlyZero())
		{
			if (APawn* SelfPawn = GetPawn())
			{
				Origin = SelfPawn->GetActorLocation();
				BlackboardComp->SetValueAsVector(TEXT("LastKnownLocation"), Origin);
			}
		}

		BlackboardComp->SetValueAsVector(TEXT("SearchOrigin"), Origin);
		BlackboardComp->SetValueAsBool(TEXT("ShouldSearch"), true);
		BlackboardComp->SetValueAsBool(TEXT("IsReturningHome"), false);

		// Initialize scan params
		constexpr float SearchDuration = 3.0f;
		constexpr float SearchHalfAngleDeg = 70.0f;

		const APawn* SelfPawn = GetPawn();
		const float CenterYaw = SelfPawn ? SelfPawn->GetActorRotation().Yaw : GetControlRotation().Yaw;

		BlackboardComp->SetValueAsFloat(TEXT("SearchEndTime"), Now + SearchDuration);
		BlackboardComp->SetValueAsFloat(TEXT("SearchMinYaw"), CenterYaw - SearchHalfAngleDeg);
		BlackboardComp->SetValueAsFloat(TEXT("SearchMaxYaw"), CenterYaw + SearchHalfAngleDeg);
		BlackboardComp->SetValueAsInt(TEXT("SearchDir"), 0);
	}
}

static bool IC_IsActorCurrentlyPerceived(UAIPerceptionComponent* Perc, AActor* Actor)
{
	if (!Perc || !Actor) return false;

	FActorPerceptionBlueprintInfo Info;
	Perc->GetActorsPerception(Actor, Info);

	for (const FAIStimulus& S : Info.LastSensedStimuli)
	{
		if (S.WasSuccessfullySensed())
		{
			return true;
		}
	}
	return false;
}

void AIroncladEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!IsValid(Actor))
	{
		return;
	}

	// We only care about the player for this card.
	ACharacter* PlayerChar = GetWorld() ? GetWorld()->GetFirstPlayerController() ? GetWorld()->GetFirstPlayerController()->GetCharacter() : nullptr : nullptr;
	if (!PlayerChar || Actor != PlayerChar)
	{
		return;
	}

	const FString SenseName = Stimulus.Type.IsValid() ? Stimulus.Type.Name.ToString() : FString(TEXT("UnknownSense"));
	const APawn* SelfPawn = GetPawn();
	const FVector SelfLoc = SelfPawn ? SelfPawn->GetActorLocation() : FVector::ZeroVector;
	const FVector TargetLoc = Actor->GetActorLocation();
	const float Dist = FVector::Dist(SelfLoc, TargetLoc);

	if (Stimulus.WasSuccessfullySensed())
	{
		UE_LOG(LogIroncladPerception, Log,
			TEXT("%s ACQUIRED Target=%s Sense=%s Dist=%.0f StimStrength=%.2f Age=%.2f"),
			*IC_PercPrefix(this),
			*GetNameSafe(Actor),
			*SenseName,
			Dist,
			Stimulus.Strength,
			Stimulus.GetAge());

		SetCurrentTarget(Actor, FString::Printf(TEXT("%s Sensed at (%.0f)"), *SenseName, Stimulus.Strength));
	}
	else
	{
		const bool bStillPerceived = IC_IsActorCurrentlyPerceived(AIPerception, Actor);

		UE_LOG(LogIroncladPerception, Log,
			TEXT("%s LOST Target=%s Sense=%s Dist=%.0f StimStrength=%.2f Age=%.2f"),
			*IC_PercPrefix(this),
			*GetNameSafe(Actor),
			*SenseName,
			Dist,
			Stimulus.Strength,
			Stimulus.GetAge());

		// Only clear if *no* sense is currently perceiving the actor.
		if (!bStillPerceived)
		{
			// “Lost” can fire because of sight loss, stimulus aging, etc.
			ClearCurrentTarget(FString::Printf(TEXT("%s Lost/Expired"), *SenseName));
		}
	}
}

void AIroncladEnemyAIController::SetCurrentTarget(AActor* NewTarget, const FString& Reason)
{
	if (CurrentTarget == NewTarget)
	{
		return;
	}

	CurrentTarget = NewTarget;
}

void AIroncladEnemyAIController::ClearCurrentTarget(const FString& Reason)
{
	if (!CurrentTarget)
	{
		return;
	}

	CurrentTarget = nullptr;
}

void AIroncladEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!BehaviorTreeAsset)
	{
		UE_LOG(LogIroncladPerception, Warning, TEXT("%s Missing BehaviorTreeAsset"), *IC_PercPrefix(this));
		return;
	}

	UBlackboardComponent* BB = nullptr;
	if (!UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BB) || !BB)
	{
		UE_LOG(LogIroncladPerception, Error, TEXT("%s UseBlackboard failed"), *IC_PercPrefix(this));
		return;
	}

	BlackboardComp = BB;

	// Set HomeLocation once
	BlackboardComp->SetValueAsVector(TEXT("HomeLocation"), InPawn->GetActorLocation());
	BlackboardComp->SetValueAsFloat(TEXT("AttackRange"), 200.f);

	RunBehaviorTree(BehaviorTreeAsset);

	// --- Set archetype based on possessed pawn class ---
	uint8 ArchetypeValue = 0; // Aggressor default
	if (Cast<AIroncladControllerEnemy>(InPawn))
	{
		ArchetypeValue = 1; // Controller
	}

	BlackboardComp->SetValueAsEnum(BBKey_EnemyArchetype, ArchetypeValue);
}