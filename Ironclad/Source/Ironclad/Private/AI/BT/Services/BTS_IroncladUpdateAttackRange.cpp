#include "AI/BT/Services/BTS_IroncladUpdateAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"

UBTS_IroncladUpdateAttackRange::UBTS_IroncladUpdateAttackRange()
{
	NodeName = TEXT("Ironclad: Update Attack Range");
	bNotifyTick = true;
	Interval = 0.15f;
	RandomDeviation = 0.0f;

	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_IroncladUpdateAttackRange, TargetActorKey), AActor::StaticClass());
	AttackRangeKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_IroncladUpdateAttackRange, AttackRangeKey));
	InAttackRangeKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_IroncladUpdateAttackRange, InAttackRangeKey));
}

void UBTS_IroncladUpdateAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AIC = OwnerComp.GetAIOwner();
	APawn* Pawn = AIC ? AIC->GetPawn() : nullptr;
	if (!BB || !Pawn)
	{
		return;
	}

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!Target)
	{
		BB->SetValueAsBool(InAttackRangeKey.SelectedKeyName, false);
		return;
	}

	float Range = BB->GetValueAsFloat(AttackRangeKey.SelectedKeyName);
	if (Range <= 0.0f)
	{
		Range = FallbackAttackRange;
	}

	const float Dist = FVector::Dist(Pawn->GetActorLocation(), Target->GetActorLocation());
	const bool bInRange = (Dist <= Range);

	BB->SetValueAsBool(InAttackRangeKey.SelectedKeyName, bInRange);
}
