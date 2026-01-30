#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_IroncladUpdateController.generated.h"

UCLASS()
class IRONCLAD_API UBTS_IroncladUpdateController : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTS_IroncladUpdateController();

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
	FBlackboardKeySelector HasLineOfSightKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector ShouldPlaceZoneKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector NextZoneTimeKey;

	// Tuning (can be overridden per BT service instance)
	UPROPERTY(EditAnywhere, Category = "Controller Tuning")
	float ZoneTriggerInnerRange = 900.0f;

	UPROPERTY(EditAnywhere, Category = "Controller Tuning")
	float ZoneCooldownSeconds = 8.0f;

	UPROPERTY(EditAnywhere, Category = "Controller Tuning")
	bool bRequireLineOfSightForZone = false;
};
