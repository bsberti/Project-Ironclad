#pragma once

#include "CoreMinimal.h"
#include "UI/IroncladEnemyHealthBarWidget.h"
#include "Characters/IroncladCharacterBase.h"
#include "IroncladEnemyBase.generated.h"

class UWidgetComponent;
class UUserWidget;

UENUM(BlueprintType)
enum class EIroncladFaction : uint8
{
	Hostile UMETA(DisplayName = "Hostile"),
	Neutral UMETA(DisplayName = "Neutral"),
	Friendly UMETA(DisplayName = "Friendly"),
};

UCLASS(Abstract)
class IRONCLAD_API AIroncladEnemyBase : public AIroncladCharacterBase
{
	GENERATED_BODY()

public:
	AIroncladEnemyBase();

	// Enemy identity (used later by targeting/AI rules)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ironclad|Enemy")
	EIroncladFaction Faction = EIroncladFaction::Hostile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ironclad|Spawn")
	FName SpawnId = NAME_None;

	UFUNCTION(BlueprintCallable, Category = "Ironclad|Spawn")
	void SetSpawnId(FName InId) { SpawnId = InId; }

protected:
	virtual void OnHitReceived_Implementation();

	// Override shared death baseline to allow enemy-specific policy
	virtual void HandleDeath() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	// --- Health bar component ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> EnemyHealthBarWidget;

	// Assign in BP defaults (once). All enemy child BPs inherit.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UIroncladEnemyHealthBarWidget> EnemyHealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	float HealthBarHideDelay = 2.0f;

	FTimerHandle HealthBarHideTimer;

	float LastHealth = 0.f;

	// Cached vitals (optional)
	UPROPERTY()
	TObjectPtr<UIroncladVitalsComponent> CachedVitals;

private:
	UFUNCTION()
	void HandleHealthChanged(float Current, float Max);

	void ShowHealthBar(float Current, float Max);
	void HideHealthBar();
	void UpdateHealthBar(float Current, float Max);
};
