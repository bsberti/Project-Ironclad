#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetFocusActor.generated.h"

UCLASS()
class IRONCLAD_API UBTTask_SetFocusActor : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SetFocusActor();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector FocusActorKey;
};
