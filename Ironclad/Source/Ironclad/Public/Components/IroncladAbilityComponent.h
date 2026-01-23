#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IroncladAbilityComponent.generated.h"

class AIroncladCharacterBase;
class UIroncladAbilityDataAsset;

USTRUCT(BlueprintType)
struct FIroncladAbilityActivationResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly) bool bActivated = false;
	UPROPERTY(BlueprintReadOnly) FName FailReason = NAME_None;
};

UCLASS(ClassGroup = (Ironclad), meta = (BlueprintSpawnableComponent))
class IRONCLAD_API UIroncladAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UIroncladAbilityComponent();

	// Main entry point
	UFUNCTION(BlueprintCallable, Category = "Ironclad|Ability")
	FIroncladAbilityActivationResult TryActivateAbility(UIroncladAbilityDataAsset* Ability, AActor* OptionalTarget = nullptr);

	// Called by AnimNotify when ExecutionMode == MontageNotify
	UFUNCTION(BlueprintCallable, Category = "Ironclad|Ability")
	void ExecutePendingAbility();

	// Cooldown readout
	UFUNCTION(BlueprintCallable, Category = "Ironclad|Ability")
	float GetRemainingCooldown(const UIroncladAbilityDataAsset* Ability) const;

	UPROPERTY(EditDefaultsOnly, Category = "Ironclad|Ability|Debug")
	bool bEnableAbilityDebug = true;

protected:
	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<AIroncladCharacterBase> OwnerChar;

	// Cooldowns: AbilityId -> NextReadyTime (world seconds)
	TMap<FName, double> NextReadyTimeByAbility;

	// Pending ability execution for montage notify mode
	UPROPERTY()
	TObjectPtr<UIroncladAbilityDataAsset> PendingAbility = nullptr;

	UPROPERTY()
	TObjectPtr<AActor> PendingTarget = nullptr;

	bool CanActivate(UIroncladAbilityDataAsset* Ability, FName& OutFailReason) const;
	void Commit(UIroncladAbilityDataAsset* Ability);
	void Execute(UIroncladAbilityDataAsset* Ability, AActor* OptionalTarget);

	void DebugPrint(UIroncladAbilityDataAsset* Ability, const FIroncladAbilityActivationResult& Result) const;
};
