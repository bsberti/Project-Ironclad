#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_IroncladRotateScan.generated.h"

UCLASS()
class IRONCLAD_API UBTTask_IroncladRotateScan : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_IroncladRotateScan();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Ironclad|Scan")
	float TurnRateDegPerSec = 360.0f;

	// If true, uses BB bounds/end time that controller set (SearchMinYaw/SearchMaxYaw/SearchEndTime/SearchDir).
	// If false, computes bounds at start (CenterYaw +/- HalfAngle) and writes them to BB.
	UPROPERTY(EditAnywhere, Category = "Ironclad|Scan")
	bool bUseBlackboardBounds = true;

	UPROPERTY(EditAnywhere, Category = "Ironclad|Scan", meta = (EditCondition = "!bUseBlackboardBounds"))
	float DurationSeconds = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Ironclad|Scan", meta = (EditCondition = "!bUseBlackboardBounds"))
	float HalfAngleDeg = 70.0f;

	// BB keys
	UPROPERTY(EditAnywhere, Category = "Ironclad|Blackboard")
	FName SearchEndTimeKey = TEXT("SearchEndTime");

	UPROPERTY(EditAnywhere, Category = "Ironclad|Blackboard")
	FName SearchMinYawKey = TEXT("SearchMinYaw");

	UPROPERTY(EditAnywhere, Category = "Ironclad|Blackboard")
	FName SearchMaxYawKey = TEXT("SearchMaxYaw");

	UPROPERTY(EditAnywhere, Category = "Ironclad|Blackboard")
	FName SearchDirKey = TEXT("SearchDir");
};
