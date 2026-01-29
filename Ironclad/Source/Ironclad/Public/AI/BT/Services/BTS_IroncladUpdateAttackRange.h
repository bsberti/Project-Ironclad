#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_IroncladUpdateAttackRange.generated.h"

UCLASS()
class IRONCLAD_API UBTS_IroncladUpdateAttackRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTS_IroncladUpdateAttackRange();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// Blackboard keys
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector InAttackRangeKey;
};
