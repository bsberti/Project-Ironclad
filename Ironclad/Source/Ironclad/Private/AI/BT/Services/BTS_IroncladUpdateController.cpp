#include "AI/BT/Services/BTS_IroncladUpdateController.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"

UBTS_IroncladUpdateController::UBTS_IroncladUpdateController()
{
	NodeName = TEXT("Ironclad: Controller Update Tactics");
	bNotifyTick = true;
	Interval = 0.2f;
	RandomDeviation = 0.05f;

	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_IroncladUpdateController, TargetActorKey), AActor::StaticClass());
	DistanceToTargetKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_IroncladUpdateController, DistanceToTargetKey));
	DesiredRangeKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_IroncladUpdateController, DesiredRangeKey));
	RangeToleranceKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_IroncladUpdateController, RangeToleranceKey));

	HasLineOfSightKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_IroncladUpdateController, HasLineOfSightKey));
	ShouldPlaceZoneKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_IroncladUpdateController, ShouldPlaceZoneKey));
	NextZoneTimeKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_IroncladUpdateController, NextZoneTimeKey));
}

void UBTS_IroncladUpdateController::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AIC = OwnerComp.GetAIOwner();
	APawn* Pawn = AIC ? AIC->GetPawn() : nullptr;
	if (!BB || !Pawn) return;

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!Target)
	{
		BB->SetValueAsBool(HasLineOfSightKey.SelectedKeyName, false);
		BB->SetValueAsBool(ShouldPlaceZoneKey.SelectedKeyName, false);
		return;
	}

	const bool bLOS = AIC ? AIC->LineOfSightTo(Target) : false;
	BB->SetValueAsBool(HasLineOfSightKey.SelectedKeyName, bLOS);

	// Pull distance (you already set this in BTS_IroncladUpdateTargetMetrics)
	const float Dist = BB->GetValueAsFloat(DistanceToTargetKey.SelectedKeyName);

	// Zone cooldown gate
	const float Now = Pawn->GetWorld() ? Pawn->GetWorld()->GetTimeSeconds() : 0.0f;
	const float NextReady = BB->GetValueAsFloat(NextZoneTimeKey.SelectedKeyName);

	const bool bCooldownReady = (Now >= NextReady);
	const bool bInTriggerRange = (Dist <= ZoneTriggerInnerRange);
	const bool bZoneLOSOK = (!bRequireLineOfSightForZone) || bLOS;

	UE_LOG(LogTemp, Warning,
		TEXT("[BTS_IroncladUpdateController] Pawn=%s Target=%s Dist=%.0f CooldownReady=%d InRange=%d LOSOK=%d => ShouldZone=%d"),
		*Pawn->GetName(),
		*Target->GetName(),
		Dist,
		bCooldownReady ? 1 : 0,
		bInTriggerRange ? 1 : 0,
		bZoneLOSOK ? 1 : 0,
		(bCooldownReady && bInTriggerRange && bZoneLOSOK) ? 1 : 0
	);

	const bool bShouldZone = bCooldownReady && bInTriggerRange && bZoneLOSOK;

	BB->SetValueAsBool(ShouldPlaceZoneKey.SelectedKeyName, bShouldZone);

	// OPTIONAL: If you want the service to also write desired range/tolerance from BB defaults,
	// you can set these in the Controller enemy BeginPlay or via BT.
	// This service doesn't force them; it just relies on them existing for other nodes.
}
