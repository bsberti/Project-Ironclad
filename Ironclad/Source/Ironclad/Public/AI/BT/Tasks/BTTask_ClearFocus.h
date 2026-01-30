#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ClearFocus.generated.h"

UCLASS()
class IRONCLAD_API UBTTask_ClearFocus : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ClearFocus();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
