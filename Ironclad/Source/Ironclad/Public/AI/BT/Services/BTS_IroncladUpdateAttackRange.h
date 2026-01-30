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

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector AttackRangeKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector InAttackRangeKey;

	// Optional safety if AttackRange is unset/0
	UPROPERTY(EditAnywhere, Category = "Tuning")
	float FallbackAttackRange = 150.0f;
};
