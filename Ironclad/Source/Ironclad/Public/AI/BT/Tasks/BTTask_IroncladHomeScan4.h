#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_IroncladHomeScan4.generated.h"

struct FHomeScanMemory
{
	float BaseYaw = 0.0f;
	float StepEndTime = 0.0f;
	bool bInitialized = false;
};

UCLASS()
class IRONCLAD_API UBTTask_IroncladHomeScan4 : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_IroncladHomeScan4();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FHomeScanMemory); }

	UPROPERTY(EditAnywhere, Category = "Ironclad|Scan")
	float TurnRateDegPerSec = 540.0f;

	UPROPERTY(EditAnywhere, Category = "Ironclad|Scan")
	float HoldSecondsPerDirection = 0.55f;

	UPROPERTY(EditAnywhere, Category = "Ironclad|Blackboard")
	FName HomeScanIndexKey = TEXT("HomeScanIndex");
};
