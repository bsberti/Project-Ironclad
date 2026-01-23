#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Combat/Damage/IroncladDamageTypes.h"
#include "IroncladAbilityDataAsset.generated.h"

UENUM(BlueprintType)
enum class EIroncladAbilityExecutionMode : uint8
{
	Instant UMETA(DisplayName = "Instant"),
	MontageNotify UMETA(DisplayName = "Montage Notify (Execute on Notify)")
};

UENUM(BlueprintType)
enum class EIroncladAbilityEffectType : uint8
{
	StaminaBurst UMETA(DisplayName = "Stamina Burst"),
	ApplyDamage   UMETA(DisplayName = "Apply Damage")
};

UCLASS(BlueprintType)
class IRONCLAD_API UIroncladAbilityDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// Stable identifier for cooldown maps, UI, etc.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FName AbilityId = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FGameplayTagContainer AbilityTags;

	// Cost / cooldown
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Rules", meta = (ClampMin = "0.0"))
	float StaminaCost = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Rules", meta = (ClampMin = "0.0"))
	float CooldownSeconds = 0.0f;

	// Animation hook
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Animation")
	TObjectPtr<UAnimMontage> ActivationMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Animation")
	EIroncladAbilityExecutionMode ExecutionMode = EIroncladAbilityExecutionMode::Instant;

	// Effect payload (first pass)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Effect")
	EIroncladAbilityEffectType EffectType = EIroncladAbilityEffectType::StaminaBurst;

	// Stamina Burst payload
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Effect", meta = (ClampMin = "0.0"))
	float StaminaRestoreAmount = 25.0f;

	// Damage payload (for later / optional now)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Effect")
	FIroncladDamageSpec DamageTemplate;
};
