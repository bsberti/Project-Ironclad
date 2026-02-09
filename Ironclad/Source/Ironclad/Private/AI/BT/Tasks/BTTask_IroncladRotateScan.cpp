#include "AI/BT/Tasks/BTTask_IroncladRotateScan.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_IroncladRotateScan::UBTTask_IroncladRotateScan()
{
	NodeName = TEXT("Ironclad Rotate Scan (Back/Forth)");
	bNotifyTick = true;
}

static float IC_NormalizeYaw(float Yaw)
{
	return FRotator::NormalizeAxis(Yaw);
}

static float IC_MoveTowardAngle(float CurrentYaw, float TargetYaw, float MaxDelta)
{
	const float Delta = FMath::FindDeltaAngleDegrees(CurrentYaw, TargetYaw);
	const float Step = FMath::Clamp(Delta, -MaxDelta, MaxDelta);
	return IC_NormalizeYaw(CurrentYaw + Step);
}

EBTNodeResult::Type UBTTask_IroncladRotateScan::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;

	AAIController* AI = OwnerComp.GetAIOwner();
	APawn* Pawn = AI ? AI->GetPawn() : nullptr;
	if (!AI || !Pawn) return EBTNodeResult::Failed;

	const float Now = Pawn->GetWorld() ? Pawn->GetWorld()->GetTimeSeconds() : 0.0f;

	float EndTime = BB->GetValueAsFloat(SearchEndTimeKey);
	float MinYaw = BB->GetValueAsFloat(SearchMinYawKey);
	float MaxYaw = BB->GetValueAsFloat(SearchMaxYawKey);

	const bool bInvalid = (EndTime <= Now) || (FMath::IsNearlyZero(MinYaw) && FMath::IsNearlyZero(MaxYaw));
	if (!bUseBlackboardBounds || bInvalid)
	{
		const float CenterYaw = Pawn->GetActorRotation().Yaw;
		BB->SetValueAsFloat(SearchMinYawKey, IC_NormalizeYaw(CenterYaw - HalfAngleDeg));
		BB->SetValueAsFloat(SearchMaxYawKey, IC_NormalizeYaw(CenterYaw + HalfAngleDeg));
		BB->SetValueAsFloat(SearchEndTimeKey, Now + DurationSeconds);
		BB->SetValueAsInt(SearchDirKey, 0);
	}

	// Ensure we stop focusing something while scanning (optional but recommended)
	AI->ClearFocus(EAIFocusPriority::Gameplay);

	return EBTNodeResult::InProgress;
}

void UBTTask_IroncladRotateScan::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AI = OwnerComp.GetAIOwner();
	APawn* Pawn = AI ? AI->GetPawn() : nullptr;

	if (!BB || !AI || !Pawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const float Now = Pawn->GetWorld() ? Pawn->GetWorld()->GetTimeSeconds() : 0.0f;
	const float EndTime = BB->GetValueAsFloat(SearchEndTimeKey);

	if (Now >= EndTime)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	const float MinYaw = IC_NormalizeYaw(BB->GetValueAsFloat(SearchMinYawKey));
	const float MaxYaw = IC_NormalizeYaw(BB->GetValueAsFloat(SearchMaxYawKey));
	int32 Dir = BB->GetValueAsInt(SearchDirKey); // 0->min, 1->max

	const float TargetYaw = (Dir == 0) ? MinYaw : MaxYaw;

	const float CurrentYaw = AI->GetControlRotation().Yaw;
	const float MaxStep = TurnRateDegPerSec * DeltaSeconds;
	const float NewYaw = IC_MoveTowardAngle(CurrentYaw, TargetYaw, MaxStep);

	FRotator NewRot = AI->GetControlRotation();
	NewRot.Yaw = NewYaw;
	AI->SetControlRotation(NewRot);

	// Flip direction when we reach the bound (within tolerance)
	const float Remaining = FMath::Abs(FMath::FindDeltaAngleDegrees(NewYaw, TargetYaw));
	if (Remaining <= 2.0f)
	{
		Dir = (Dir == 0) ? 1 : 0;
		BB->SetValueAsInt(SearchDirKey, Dir);
	}
}
