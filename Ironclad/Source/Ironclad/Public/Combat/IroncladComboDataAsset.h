#pragma once

#include "Combat/IroncladCombatTypes.h"

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IroncladComboDataAsset.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType)
struct IRONCLAD_API FIroncladComboStep
{
    GENERATED_BODY()

    // Which input starts this step (use your ECombatAction)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    ECombatAction Input = ECombatAction::LightAttack;

    // Montage to play (required)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UAnimMontage> Montage = nullptr;

    // Optional montage section (NAME_None = play from start)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName SectionName = NAME_None;

    // Recovery seconds to pass to StartAttackMontage()
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float RecoverySeconds = 0.25f;

    // Deterministic next step indices
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 NextOnLight = INDEX_NONE;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 NextOnHeavy = INDEX_NONE;

    // How long after this step starts we allow continuing before combo resets
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float ResetTimeoutSeconds = 1.0f;

    // Optional debug label
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName DebugStepName = NAME_None;
};

UCLASS(BlueprintType)
class IRONCLAD_API UIroncladComboDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FIroncladComboStep> Steps;

    const FIroncladComboStep* GetStep(int32 Index) const
    {
        return Steps.IsValidIndex(Index) ? &Steps[Index] : nullptr;
    }
};
