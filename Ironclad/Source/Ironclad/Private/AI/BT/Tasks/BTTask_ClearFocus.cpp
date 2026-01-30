#include "AI/BT/Tasks/BTTask_ClearFocus.h"

#include "AIController.h"

UBTTask_ClearFocus::UBTTask_ClearFocus()
{
	NodeName = TEXT("Ironclad: Clear Focus");
}

EBTNodeResult::Type UBTTask_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC)
	{
		return EBTNodeResult::Failed;
	}

	AIC->ClearFocus(EAIFocusPriority::Gameplay);
	return EBTNodeResult::Succeeded;
}
