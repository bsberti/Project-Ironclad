#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_IroncladAggressorAttack.generated.h"

UCLASS()
class IRONCLAD_API UBTTask_IroncladAggressorAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_IroncladAggressorAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;
};
