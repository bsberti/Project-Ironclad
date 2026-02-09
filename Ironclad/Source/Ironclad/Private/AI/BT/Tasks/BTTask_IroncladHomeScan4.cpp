#include "AI/BT/Tasks/BTTask_IroncladHomeScan4.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"

UBTTask_IroncladHomeScan4::UBTTask_IroncladHomeScan4()
{
	NodeName = TEXT("Ironclad Home Scan (4 Directions)");
	bNotifyTick = true;
}

static float IC_NormalizeYaw(float Yaw) { return FRotator::NormalizeAxis(Yaw); }
static float IC_MoveTowardAngle(float CurrentYaw, float TargetYaw, float MaxDelta)
{
	const float Delta = FMath::FindDeltaAngleDegrees(CurrentYaw, TargetYaw);
	const float Step = FMath::Clamp(Delta, -MaxDelta, MaxDelta);
	return IC_NormalizeYaw(CurrentYaw + Step);
}

EBTNodeResult::Type UBTTask_IroncladHomeScan4::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AI = OwnerComp.GetAIOwner();
	APawn* Pawn = AI ? AI->GetPawn() : nullptr;

	if (!BB || !AI || !Pawn) return EBTNodeResult::Failed;

	AI->ClearFocus(EAIFocusPriority::Gameplay);

	// Allocate memory
	FHomeScanMemory* Mem = (FHomeScanMemory*)NodeMemory;
	Mem->BaseYaw = Pawn->GetActorRotation().Yaw;
	Mem->StepEndTime = (Pawn->GetWorld() ? Pawn->GetWorld()->GetTimeSeconds() : 0.0f) + HoldSecondsPerDirection;
	Mem->bInitialized = true;

	BB->SetValueAsInt(HomeScanIndexKey, 0);

	return EBTNodeResult::InProgress;
}

void UBTTask_IroncladHomeScan4::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AI = OwnerComp.GetAIOwner();
	APawn* Pawn = AI ? AI->GetPawn() : nullptr;

	if (!BB || !AI || !Pawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FHomeScanMemory* Mem = (FHomeScanMemory*)NodeMemory;
	if (!Mem->bInitialized)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const float Now = Pawn->GetWorld() ? Pawn->GetWorld()->GetTimeSeconds() : 0.0f;
	int32 Index = BB->GetValueAsInt(HomeScanIndexKey);

	if (Index >= 4)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	const float TargetYaw = IC_NormalizeYaw(Mem->BaseYaw + 90.0f * Index);

	const float CurrentYaw = AI->GetControlRotation().Yaw;
	const float MaxStep = TurnRateDegPerSec * DeltaSeconds;
	const float NewYaw = IC_MoveTowardAngle(CurrentYaw, TargetYaw, MaxStep);

	FRotator NewRot = AI->GetControlRotation();
	NewRot.Yaw = NewYaw;
	AI->SetControlRotation(NewRot);

	// We "hold" a direction for HoldSecondsPerDirection; during the hold we keep rotating toward target yaw.
	if (Now >= Mem->StepEndTime)
	{
		Index++;
		BB->SetValueAsInt(HomeScanIndexKey, Index);
		Mem->StepEndTime = Now + HoldSecondsPerDirection;
	}
}
