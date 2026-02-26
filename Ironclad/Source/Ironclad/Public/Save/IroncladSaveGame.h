#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Save/IroncladSaveTypes.h"
#include "IroncladSaveGame.generated.h"

UCLASS()
class IRONCLAD_API UIroncladSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 SaveVersion = 1;

	UPROPERTY()
	FIroncladPlayerSaveData Player;

	UPROPERTY()
	FIroncladArenaSaveData Arena;
};