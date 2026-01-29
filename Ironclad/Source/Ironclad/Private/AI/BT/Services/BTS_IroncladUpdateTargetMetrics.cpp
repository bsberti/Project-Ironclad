#include "AI/BT/Services/BTS_IroncladUpdateTargetMetrics.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/IroncladCharacterBase.h"

#include "Components/IroncladVitalsComponent.h"

#include "AIController.h"
#include "GameFramework/Pawn.h"

UBTS_IroncladUpdateTargetMetrics::UBTS_IroncladUpdateTargetMetrics()
{
	NodeName = TEXT("Ironclad: Update Target Metrics");
	bNotifyTick = true;
	Interval = 0.2f;
	RandomDeviation = 0.05f;

	// Strongly helps editor filtering + prevents wrong key types
	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_IroncladUpdateTargetMetrics, TargetActorKey), AActor::StaticClass());
	DistanceToTargetKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_IroncladUpdateTargetMetrics, DistanceToTargetKey));
	LastKnownLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_IroncladUpdateTargetMetrics, LastKnownLocationKey));
}

void UBTS_IroncladUpdateTargetMetrics::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AIC = OwnerComp.GetAIOwner();
	APawn* Pawn = AIC ? AIC->GetPawn() : nullptr;
	if (!BB || !Pawn) return;

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!Target)
	{
		BB->SetValueAsFloat(DistanceToTargetKey.SelectedKeyName, TNumericLimits<float>::Max());
		return;
	}

	AIroncladCharacterBase* TargetChar = Cast<AIroncladCharacterBase>(Target);
	if (TargetChar)
	{
		UIroncladVitalsComponent* Vitals = TargetChar->GetVitals();
		if (Vitals && Vitals->IsDead())
		{
			BB->SetValueAsBool(HasTargetKey.SelectedKeyName, false);
			BB->ClearValue(TargetActorKey.SelectedKeyName);

			// Reset metrics to avoid stale “in range” conditions
			BB->SetValueAsFloat(DistanceToTargetKey.SelectedKeyName, TNumericLimits<float>::Max());

			if (AIC)
			{
				AIC->StopMovement();
				AIC->ClearFocus(EAIFocusPriority::Gameplay);
			}
			return;
		}
	}

	const float Dist = FVector::Dist(Pawn->GetActorLocation(), Target->GetActorLocation());
	BB->SetValueAsFloat(DistanceToTargetKey.SelectedKeyName, Dist);
	BB->SetValueAsVector(LastKnownLocationKey.SelectedKeyName, Target->GetActorLocation());
}
