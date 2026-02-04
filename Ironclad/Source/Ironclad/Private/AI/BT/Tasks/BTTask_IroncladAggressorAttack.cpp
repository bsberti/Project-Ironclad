#include "AI/BT/Tasks/BTTask_IroncladAggressorAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/IroncladAggressorEnemy.h"

UBTTask_IroncladAggressorAttack::UBTTask_IroncladAggressorAttack()
{
	NodeName = TEXT("Aggressor Attack");
}

EBTNodeResult::Type UBTTask_IroncladAggressorAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) {
		UE_LOG(LogTemp, Warning, TEXT("BTTask_IroncladAggressorAttack: AIC is null"));
		return EBTNodeResult::Failed;
	}

	AIroncladAggressorEnemy* Enemy = Cast<AIroncladAggressorEnemy>(AIC->GetPawn());
	if (!Enemy) {
		UE_LOG(LogTemp, Warning, TEXT("BTTask_IroncladAggressorAttack: Enemy pawn is null or not of type AIroncladAggressorEnemy"));
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) {
		UE_LOG(LogTemp, Warning, TEXT("BTTask_IroncladAggressorAttack: Blackboard component is null"));
		return EBTNodeResult::Failed;
	}

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!Target) {
		UE_LOG(LogTemp, Warning, TEXT("BTTask_IroncladAggressorAttack: Target actor is null or invalid"));
		return EBTNodeResult::Failed;
	}

	if (Enemy->IsDead())
	{
		return EBTNodeResult::Failed;
	}

	const bool bStarted = Enemy->TryStartAttack(Target);
	return bStarted ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
