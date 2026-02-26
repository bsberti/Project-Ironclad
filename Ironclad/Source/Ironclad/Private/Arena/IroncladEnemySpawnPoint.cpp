#include "Arena/IroncladEnemySpawnPoint.h"
#include "Characters/IroncladCharacterBase.h"

AIroncladEnemySpawnPoint::AIroncladEnemySpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;
	SetActorHiddenInGame(true); // marker only
}

#if WITH_EDITOR
void AIroncladEnemySpawnPoint::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Nice-to-have: auto-fill SpawnId if empty
	if (SpawnId.IsNone())
	{
		SpawnId = FName(*GetName());
	}
}
#endif