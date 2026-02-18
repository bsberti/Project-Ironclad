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

namespace 
{
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
}

EBTNodeResult::Type UBTTask_IroncladRotateScan::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;

	UE_LOG(LogTemp, Warning, TEXT("RotateScan START: EndTime=%.2f MinYaw=%.1f MaxYaw=%.1f"),
		BB->GetValueAsFloat(TEXT("SearchEndTime")),
		BB->GetValueAsFloat(TEXT("SearchMinYaw")),
		BB->GetValueAsFloat(TEXT("SearchMaxYaw")));

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
		// PSEUDOCODE / PLAN:
		// 1. Read center yaw from pawn rotation (float).
		// 2. Compute min and max yaw by subtracting/adding HalfAngleDeg.
		// 3. Cast the computed value to float to avoid overload ambiguity when calling IC_NormalizeYaw.
		// 4. Store normalized values into blackboard keys.
		// Note: Apply explicit cast on both min and max calculations to ensure the single-float overload is selected.

		const float CenterYaw = Pawn->GetActorRotation().Yaw;
		BB->SetValueAsFloat(SearchMinYawKey, IC_NormalizeYaw(static_cast<float>(CenterYaw - HalfAngleDeg)));
		BB->SetValueAsFloat(SearchMaxYawKey, IC_NormalizeYaw(static_cast<float>(CenterYaw + HalfAngleDeg)));
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

	static float LastLog = 0.0f;
	if (Now - LastLog > 0.5f)
	{
		LastLog = Now;
		UE_LOG(LogTemp, Warning, TEXT("RotateScan TICK: Now=%.2f EndTime=%.2f Dir=%d"),
			Now,
			BB->GetValueAsFloat(TEXT("SearchEndTime")),
			BB->GetValueAsInt(TEXT("SearchDir")));
	}

	if (Now >= EndTime)
	{
		// End search and start return-home immediately
		BB->SetValueAsBool(TEXT("ShouldSearch"), false);
		BB->SetValueAsBool(TEXT("IsReturningHome"), true);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	const float MinYaw = IC_NormalizeYaw(BB->GetValueAsFloat(SearchMinYawKey));
	const float MaxYaw = IC_NormalizeYaw(BB->GetValueAsFloat(SearchMaxYawKey));
	int32 Dir = BB->GetValueAsInt(SearchDirKey); // 0->min, 1->max

	//const float TargetYaw = (Dir == 0) ? MinYaw : MaxYaw; <-- This would be the original min/max approach, but it can cause stuttering at bounds. The oscillation approach is smoother.

	const float CenterYaw = BB->GetValueAsFloat(TEXT("SearchCenterYaw"));
	const float HalfAngle = 70.0f;
	const float Speed = 2.5f;

	const float StartTime = BB->GetValueAsFloat(TEXT("SearchStartTime"));
	const float T = Now - StartTime;

	const float DesiredYaw = FRotator::NormalizeAxis(CenterYaw + FMath::Sin(T * Speed) * HalfAngle);

	const float CurrentYaw = AI->GetControlRotation().Yaw;
	const float MaxStep = TurnRateDegPerSec * DeltaSeconds;

	// Move toward DesiredYaw with MaxStep (your existing IC_MoveTowardAngle)
	const float NewYaw = IC_MoveTowardAngle(CurrentYaw, DesiredYaw, MaxStep);

	//const float NewYaw = IC_MoveTowardAngle(CurrentYaw, TargetYaw, MaxStep); <-- This would be the original min/max approach, but it can cause stuttering at bounds. The oscillation approach is smoother.

	FRotator NewRot = AI->GetControlRotation();
	NewRot.Yaw = NewYaw;
	AI->SetControlRotation(NewRot);
	Pawn->SetActorRotation(FRotator(0.f, NewYaw, 0.f));

	//UE_LOG(LogTemp, Warning, TEXT("Yaw: %.1f"), AI->GetControlRotation().Yaw);
	//UE_LOG(LogTemp, Warning, TEXT("CtrlYaw=%.1f PawnYaw=%.1f"), AI->GetControlRotation().Yaw, Pawn->GetActorRotation().Yaw);

	// Flip direction when we reach the bound (within tolerance)
	const float Remaining = FMath::Abs(FMath::FindDeltaAngleDegrees(NewYaw, DesiredYaw));
	if (Remaining <= 2.0f)
	{
		Dir = (Dir == 0) ? 1 : 0;
		BB->SetValueAsInt(SearchDirKey, Dir);
	}
}
