#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_IroncladControllerMovePoint.generated.h"

UCLASS()
class IRONCLAD_API UBTS_IroncladControllerMovePoint : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTS_IroncladControllerMovePoint();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector DistanceToTargetKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector DesiredRangeKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector RangeToleranceKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector OrbitSignKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MoveToLocationKey;

	// Tuning
	UPROPERTY(EditAnywhere, Category = "Controller Tuning")
	float RetreatExtraDistance = 300.0f; // how far beyond ring we try to get when too close

	UPROPERTY(EditAnywhere, Category = "Controller Tuning")
	float OrbitLateralOffset = 450.0f; // strafe/orbit width
};
