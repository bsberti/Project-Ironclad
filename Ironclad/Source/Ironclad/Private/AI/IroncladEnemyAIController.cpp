#include "AI/IroncladEnemyAIController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"

#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogIroncladPerception, Log, All);

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
	FString Names;
	for (AActor* A : UpdatedActors)
	{
		if (!Names.IsEmpty()) Names += TEXT(", ");
		Names += GetNameSafe(A);
	}

	UE_LOG(LogIroncladPerception, Display, TEXT("%s UpdatedActors=%d [%s]"),
		*IC_PercPrefix(this), UpdatedActors.Num(), *Names);
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
