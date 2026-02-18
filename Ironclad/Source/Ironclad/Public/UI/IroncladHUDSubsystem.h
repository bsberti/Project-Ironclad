#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "IroncladHUDSubsystem.generated.h"

class APlayerController;
class APawn;
class UIroncladHUDWidget;
class UIroncladVitalsComponent; 
class UIroncladAbilityComponent;

UCLASS()
class IRONCLAD_API UIroncladHUDSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Can be called safely; it’ll early out if not ready.
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void EnsureHUD();

private:
	// --- World / Player wiring ---
	void TryBindToLocalPlayer();

	UFUNCTION()
	void HandlePawnChanged(APawn* OldPawn, APawn* NewPawn);

	// --- Vitals wiring ---
	void BindVitalsFromPawn(APawn* Pawn);
	void UnbindVitals();

	// --- Abilities wiring ---
	void BindAbilitiesFromPawn(APawn* Pawn);
	void UnbindAbilities();

	// --- Widget ---
	void CreateHUDWidgetIfNeeded(APlayerController* PC);

private:
	// We use UFUNCTION so we can bind to DYNAMIC delegates (BlueprintAssignable).
	UFUNCTION()
	void HandleHealthChanged(float Current, float Max);

	UFUNCTION()
	void HandleStaminaChanged(float Current, float Max);

	UFUNCTION()
	void HandleCooldownStarted(FName AbilityId, float Duration, double EndTime);

	UFUNCTION()
	void HandleCooldownEnded(FName AbilityId);

	UFUNCTION()
	void HandleDeath();

private:
	UPROPERTY()
	TObjectPtr<UIroncladHUDWidget> HUDWidget;

	UPROPERTY()
	TObjectPtr<UIroncladAbilityComponent> BoundAbilities;

	UPROPERTY()
	TObjectPtr<UIroncladVitalsComponent> BoundVitals;

	// Hard reference by path (simple for now). We can migrate to DeveloperSettings later.
	UPROPERTY()
	TSubclassOf<UIroncladHUDWidget> HUDWidgetClass;

	TWeakObjectPtr<APlayerController> CachedPC;
};
