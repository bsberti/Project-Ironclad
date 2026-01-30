#include "AI/BT/Services/BTS_IroncladControllerMovePoint.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "NavigationSystem.h"

UBTS_IroncladControllerMovePoint::UBTS_IroncladControllerMovePoint()
{
	NodeName = TEXT("Ironclad: Controller Compute Move Point");
	bNotifyTick = true;
	Interval = 0.2f;
	RandomDeviation = 0.05f;

	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_IroncladControllerMovePoint, TargetActorKey), AActor::StaticClass());
	DistanceToTargetKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_IroncladControllerMovePoint, DistanceToTargetKey));
	DesiredRangeKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_IroncladControllerMovePoint, DesiredRangeKey));
	RangeToleranceKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_IroncladControllerMovePoint, RangeToleranceKey));

	OrbitSignKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_IroncladControllerMovePoint, OrbitSignKey));
	MoveToLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_IroncladControllerMovePoint, MoveToLocationKey));
}

void UBTS_IroncladControllerMovePoint::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AIC = OwnerComp.GetAIOwner();
	APawn* Pawn = AIC ? AIC->GetPawn() : nullptr;
	if (!BB || !Pawn) return;

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!Target) return;

	const float Dist = BB->GetValueAsFloat(DistanceToTargetKey.SelectedKeyName);

	float DesiredRange = BB->GetValueAsFloat(DesiredRangeKey.SelectedKeyName);
	float Tol = BB->GetValueAsFloat(RangeToleranceKey.SelectedKeyName);

	// Safe defaults if unset in BB
	if (DesiredRange <= 1.0f) DesiredRange = 1200.0f;
	if (Tol <= 0.0f) Tol = 250.0f;

	const FVector SelfLoc = Pawn->GetActorLocation();
	const FVector TargetLoc = Target->GetActorLocation();

	FVector ToSelfFromTarget = (SelfLoc - TargetLoc);
	ToSelfFromTarget.Z = 0.0f;

	if (ToSelfFromTarget.IsNearlyZero())
	{
		ToSelfFromTarget = FVector(1, 0, 0);
	}

	const FVector AwayDir = ToSelfFromTarget.GetSafeNormal();

	// Decide retreat vs orbit
	const bool bTooClose = Dist < (DesiredRange - Tol);

	FVector DesiredPoint = SelfLoc;

	if (bTooClose)
	{
		// Retreat away from target (push out past ring a bit)
		const float Push = (DesiredRange - Dist) + RetreatExtraDistance;
		DesiredPoint = SelfLoc + AwayDir * Push;
	}
	else
	{
		// Orbit / strafe around the ring at DesiredRange
		FVector ToTarget = (TargetLoc - SelfLoc);
		ToTarget.Z = 0.0f;
		const FVector ToTargetDir = ToTarget.GetSafeNormal();

		// Perpendicular direction
		FVector Right = FVector::CrossProduct(ToTargetDir, FVector::UpVector);

		float OrbitSign = BB->GetValueAsFloat(OrbitSignKey.SelectedKeyName);
		if (FMath::IsNearlyZero(OrbitSign)) OrbitSign = 1.0f;

		const FVector Lateral = Right * OrbitSign * OrbitLateralOffset;

		DesiredPoint = TargetLoc - ToTargetDir * DesiredRange + Lateral;
	}

	// Project to navmesh
	if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Pawn->GetWorld()))
	{
		FNavLocation Projected;
		if (NavSys->ProjectPointToNavigation(DesiredPoint, Projected))
		{
			DesiredPoint = Projected.Location;
		}
	}

	BB->SetValueAsVector(MoveToLocationKey.SelectedKeyName, DesiredPoint);
}
