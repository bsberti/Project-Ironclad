#pragma once

#include "CoreMinimal.h"
#include "IroncladCombatTypes.generated.h"

UENUM(BlueprintType)
enum class ECombatState : uint8
{
    Idle,
    Attacking,
    Dodging,
    Recovering,
    Stunned,
};

UENUM(BlueprintType)
enum class ECombatAction : uint8
{
    LightAttack,
    HeavyAttack,
    Dodge,
};
