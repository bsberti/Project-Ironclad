#include "AI/BT/Tasks/BTTask_SetFocusActor.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetFocusActor::UBTTask_SetFocusActor()
{
	NodeName = TEXT("Ironclad: Set Focus (Actor)");

	FocusActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_SetFocusActor, FocusActorKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_SetFocusActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AIC || !BB)
	{
		return EBTNodeResult::Failed;
	}

	AActor* FocusActor = Cast<AActor>(BB->GetValueAsObject(FocusActorKey.SelectedKeyName));
	if (!FocusActor)
	{
		return EBTNodeResult::Failed;
	}

	AIC->SetFocus(FocusActor, EAIFocusPriority::Gameplay);
	return EBTNodeResult::Succeeded;
}
