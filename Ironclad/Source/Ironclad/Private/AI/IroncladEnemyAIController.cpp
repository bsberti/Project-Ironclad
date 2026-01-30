#include "AI/IroncladEnemyAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/IroncladPlayerCharacter.h"
#include "Characters/IroncladAggressorEnemy.h"
#include "Characters/IroncladControllerEnemy.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"

#include "GameFramework/Character.h"

static const FName BBKey_EnemyArchetype(TEXT("EnemyArchetype"));

AIroncladEnemyAIController::AIroncladEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = false;

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	// --- Sight (required) ---
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 2000.0f;
	SightConfig->LoseSightRadius = 2400.0f;
	SightConfig->PeripheralVisionAngleDegrees = 70.0f;
	SightConfig->SetMaxAge(2.0f);

	// Detect everything for now; we can filter by team/channel later if needed.
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(UAISense_Sight::StaticClass());

	// --- Hearing (optional) ---
	// If you want hearing enabled, leave this on.
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

static FString IC_PercPrefix(const AAIController* Controller)
{
	const APawn* Pawn = Controller ? Controller->GetPawn() : nullptr;

	const FString PawnName = Pawn ? Pawn->GetName() : TEXT("NoPawn");
	const FString CtrlName = Controller ? Controller->GetName() : TEXT("NoController");

	return FString::Printf(TEXT("[Perception][Ctrl=%s][Pawn=%s]"), *CtrlName, *PawnName);
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

void AIroncladEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	// Super::OnPerceptionUpdated(UpdatedActors);

	// Keep your logging
	FString Names;
	for (AActor* A : UpdatedActors)
	{
		if (!Names.IsEmpty()) Names += TEXT(", ");
		Names += GetNameSafe(A);
	}
	UE_LOG(LogIroncladPerception, Display, TEXT("%s UpdatedActors=%d [%s]"),
		*IC_PercPrefix(this), UpdatedActors.Num(), *Names);

	if (!BlackboardComp) return;

	AActor* BBTargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));

	for (AActor* Actor : UpdatedActors)
	{
		if (!Actor) continue;
		if (!Actor->IsA(AIroncladPlayerCharacter::StaticClass())) continue;

		FActorPerceptionBlueprintInfo Info;
		GetPerceptionComponent()->GetActorsPerception(Actor, Info);

		// "Sensed" means any stimulus currently active
		const bool bSensedNow = Info.LastSensedStimuli.ContainsByPredicate(
			[](const FAIStimulus& S) { return S.WasSuccessfullySensed(); }
		);

		if (bSensedNow)
		{
			BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Actor);
			BlackboardComp->SetValueAsBool(TEXT("HasTarget"), true);
			BlackboardComp->SetValueAsVector(TEXT("LastKnownLocation"), Actor->GetActorLocation());
		}
		else
		{
			// If we lost the current target, clear it
			if (Actor == BBTargetActor)
			{
				BlackboardComp->ClearValue(TEXT("TargetActor"));
				BlackboardComp->SetValueAsBool(TEXT("HasTarget"), false);
			}
		}
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