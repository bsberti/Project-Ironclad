#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IroncladEnemySpawnPoint.generated.h"

class AIroncladCharacterBase;

UCLASS()
class IRONCLAD_API AIroncladEnemySpawnPoint : public AActor
{
	GENERATED_BODY()

public:
	AIroncladEnemySpawnPoint();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ironclad|Spawn")
	FName SpawnId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ironclad|Spawn")
	TSubclassOf<AIroncladCharacterBase> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ironclad|Spawn")
	bool bSpawnOnBeginPlay = true;

	// Convenience: use actor transform as spawn transform.
	FTransform GetSpawnTransform() const { return GetActorTransform(); }

protected:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};