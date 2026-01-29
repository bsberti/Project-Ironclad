#include "AI/BT/Tasks/BTTask_IroncladSetDecisionState.h"
#include "AIController.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_IroncladSetDecisionState::UBTTask_IroncladSetDecisionState()
{
	NodeName = TEXT("Ironclad: Set Decision State");
}

EBTNodeResult::Type UBTTask_IroncladSetDecisionState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	/*UE_LOG(LogTemp, Warning, TEXT("[DecisionState] %s -> %s"),
		*GetNameSafe(OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr),
		*UEnum::GetValueAsString(StateToSet));

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("SetDecisionState ExecuteTask"));*/

	if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent())
	{
		BB->SetValueAsEnum(DecisionStateKey.SelectedKeyName, static_cast<uint8>(StateToSet));
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
