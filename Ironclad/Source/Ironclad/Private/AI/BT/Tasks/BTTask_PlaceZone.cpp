#include "AI/BT/Tasks/BTTask_PlaceZone.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"

UBTTask_PlaceZone::UBTTask_PlaceZone()
{
	NodeName = TEXT("Ironclad: Place Zone");

	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_PlaceZone, TargetActorKey), AActor::StaticClass());
	ShouldPlaceZoneKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_PlaceZone, ShouldPlaceZoneKey));
	NextZoneTimeKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_PlaceZone, NextZoneTimeKey));
}

EBTNodeResult::Type UBTTask_PlaceZone::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("UBTTask_PlaceZone: ExecuteTask called"));

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AIC = OwnerComp.GetAIOwner();
	APawn* Pawn = AIC ? AIC->GetPawn() : nullptr;
	if (!BB || !Pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_PlaceZone: Missing Blackboard or Pawn"));
		return EBTNodeResult::Failed;
	}

	if (!ZoneClass)
	{
		// Nothing to spawn
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_PlaceZone: ZoneClass is not set"));
		BB->SetValueAsBool(ShouldPlaceZoneKey.SelectedKeyName, false);
		return EBTNodeResult::Failed;
	}

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_PlaceZone: Target actor is not set or invalid"));
		BB->SetValueAsBool(ShouldPlaceZoneKey.SelectedKeyName, false);
		return EBTNodeResult::Failed;
	}

	UWorld* World = Pawn->GetWorld();
	if (!World)
	{
		return EBTNodeResult::Failed;
	}

	const float Now = World->GetTimeSeconds();
	const float NextReady = BB->GetValueAsFloat(NextZoneTimeKey.SelectedKeyName);

	// Cooldown gate (defensive — service should already handle it)
	if (Now < NextReady)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_PlaceZone: Still on cooldown (%.2f < %.2f)"), Now, NextReady);
		BB->SetValueAsBool(ShouldPlaceZoneKey.SelectedKeyName, false);
		return EBTNodeResult::Succeeded;
	}

	FVector SpawnLoc = Target->GetActorLocation();

	if (ForwardOffset != 0.0f)
	{
		const FVector Forward = Target->GetActorForwardVector();
		SpawnLoc += Forward * ForwardOffset;
	}

	// Keep it flat
	SpawnLoc.Z = Pawn->GetActorLocation().Z;

	FActorSpawnParameters Params;
	Params.Owner = Pawn;
	Params.Instigator = Pawn;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* Spawned = World->SpawnActor<AActor>(ZoneClass, SpawnLoc, FRotator::ZeroRotator, Params);
	if (!Spawned)
	{
		BB->SetValueAsBool(ShouldPlaceZoneKey.SelectedKeyName, false);
		return EBTNodeResult::Failed;
	}

	// Update cooldown + clear intent
	BB->SetValueAsFloat(NextZoneTimeKey.SelectedKeyName, Now + CooldownSeconds);
	BB->SetValueAsBool(ShouldPlaceZoneKey.SelectedKeyName, false);

	return EBTNodeResult::Succeeded;
}
