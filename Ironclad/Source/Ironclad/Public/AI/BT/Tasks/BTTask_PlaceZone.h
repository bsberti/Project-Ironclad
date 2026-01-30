#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PlaceZone.generated.h"

UCLASS()
class IRONCLAD_API UBTTask_PlaceZone : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_PlaceZone();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector ShouldPlaceZoneKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector NextZoneTimeKey;

	// Which actor to spawn (BP subclass is ideal)
	UPROPERTY(EditAnywhere, Category = "Zone")
	TSubclassOf<AActor> ZoneClass;

	// Cooldown written to BB (seconds)
	UPROPERTY(EditAnywhere, Category = "Zone|Tuning")
	float CooldownSeconds = 8.0f;

	// Simple placement: drop at target location (optionally offset forward)
	UPROPERTY(EditAnywhere, Category = "Zone|Tuning")
	float ForwardOffset = 0.0f;
};
