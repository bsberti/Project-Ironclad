#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTS_IroncladUpdateTargetMetrics.generated.h"

UCLASS(Blueprintable)
class IRONCLAD_API UBTS_IroncladUpdateTargetMetrics : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_IroncladUpdateTargetMetrics();

protected:
	// Editable in BT node Details (and in BP subclass)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FBlackboardKeySelector DistanceToTargetKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FBlackboardKeySelector LastKnownLocationKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
