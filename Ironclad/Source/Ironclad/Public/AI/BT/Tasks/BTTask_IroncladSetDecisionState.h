#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "AI/EEnemyDecisionState.h"
#include "BTTask_IroncladSetDecisionState.generated.h"

UCLASS()
class IRONCLAD_API UBTTask_IroncladSetDecisionState : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_IroncladSetDecisionState();

protected:
	UPROPERTY(EditAnywhere, Category = "Ironclad")
	EEnemyDecisionState StateToSet = EEnemyDecisionState::Idle;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector DecisionStateKey;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
