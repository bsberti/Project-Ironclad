#pragma once

#include "CoreMinimal.h"
#include "EEnemyDecisionState.generated.h"

UENUM(BlueprintType)
enum class EEnemyDecisionState : uint8
{
	Idle   UMETA(DisplayName = "Idle"),
	Chase  UMETA(DisplayName = "Chase"),
	Attack UMETA(DisplayName = "Attack"),
	Return UMETA(DisplayName = "Return"),
};
