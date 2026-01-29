#include "AI/BT/Services/BTS_IroncladUpdateAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/IroncladAggressorEnemy.h"

UBTS_IroncladUpdateAttackRange::UBTS_IroncladUpdateAttackRange()
{
	NodeName = TEXT("Update Attack Range");
	Interval = 0.15f;
	RandomDeviation = 0.0f;
}

void UBTS_IroncladUpdateAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return;

	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) return;

	AIroncladAggressorEnemy* Enemy = Cast<AIroncladAggressorEnemy>(AIC->GetPawn());
	if (!Enemy)
	{
		BB->SetValueAsBool(InAttackRangeKey.SelectedKeyName, false);
		return;
	}

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!Target)
	{
		BB->SetValueAsBool(InAttackRangeKey.SelectedKeyName, false);
		return;
	}

	const float Dist = FVector::Dist(Enemy->GetActorLocation(), Target->GetActorLocation());
	const bool bInRange = Dist <= Enemy->GetAttackRange();

	BB->SetValueAsBool(InAttackRangeKey.SelectedKeyName, bInRange);
}
