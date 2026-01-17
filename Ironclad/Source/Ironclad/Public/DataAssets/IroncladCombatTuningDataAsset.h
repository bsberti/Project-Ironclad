#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IroncladCombatTuningDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FDodgeTuning
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float StaminaCost = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float CooldownSeconds = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float IFrameSeconds = 0.f;
};

USTRUCT(BlueprintType)
struct FAttackTuning
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float StaminaCost = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float RecoverySeconds = 0.25f;
};

UCLASS(BlueprintType)
class IRONCLAD_API UIroncladCombatTuningDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FAttackTuning Light;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FAttackTuning Heavy;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FDodgeTuning Dodge;
};
