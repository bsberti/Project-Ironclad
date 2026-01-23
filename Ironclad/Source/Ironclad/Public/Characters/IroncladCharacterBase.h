#pragma once

#include "Combat/Damage/IroncladDamageable.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IroncladCharacterBase.generated.h"

class UIroncladVitalsComponent;

UENUM(BlueprintType)
enum class EIroncladHitReactionKind : uint8
{
    Light UMETA(DisplayName = "Light"),
    Heavy UMETA(DisplayName = "Heavy"),
    Stagger UMETA(DisplayName = "Stagger")
};

UCLASS()
class IRONCLAD_API AIroncladCharacterBase : public ACharacter, public IIroncladDamageable
{
    GENERATED_BODY()

public:
    AIroncladCharacterBase();

    virtual FIroncladDamageResult ApplyDamage_Implementation(const FIroncladDamageSpec& Spec) override;

    UFUNCTION(BlueprintCallable, Category = "Vitals")
    bool ApplyDamageToVitals(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Vitals")
    bool HealVitals(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Vitals")
    bool SpendStamina(float Amount);

    UFUNCTION(BlueprintPure, Category = "Vitals")
    bool IsDead() const;

    UFUNCTION(BlueprintPure, Category = "Vitals")
    UIroncladVitalsComponent* GetVitals() const { return VitalsComponent; }

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UIroncladVitalsComponent* VitalsComponent;

    // One-shot latch so we never run death twice.
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Damage")
    bool bDeathHandled = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damage")
    TObjectPtr<class UIroncladDamageReceiverComponent> DamageReceiver = nullptr;

    virtual void HandleDeath();

	// Montages (assign per-character in BP defaults)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Reactions")
	TObjectPtr<UAnimMontage> HitReactLightMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Reactions")
	TObjectPtr<UAnimMontage> HitReactHeavyMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Reactions")
	TObjectPtr<UAnimMontage> DeathMontage = nullptr;

	// Simple gating to avoid montage spam
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Reactions")
	bool bAllowReactWhileReacting = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Reactions", meta = (ClampMin = "0.0"))
	float ReactionLockSeconds = 0.25f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Damage|Reactions")
	bool bIsReacting = false;

	FTimerHandle ReactionLockTimer;

	// Optional stagger threshold (first pass)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Reactions")
	bool bEnableStagger = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Reactions", meta = (ClampMin = "0.0"))
	float StaggerDamageThreshold = 25.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Reactions", meta = (ClampMin = "0.0"))
	float StaggerWindowSeconds = 1.0f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Damage|Reactions")
	float AccumulatedDamageInWindow = 0.0f;

	FTimerHandle StaggerWindowTimer;

	// --- Stagger / Poise (refined first pass) ---

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ironclad|Stagger")
	bool bUsePoise = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ironclad|Stagger", meta = (ClampMin = "0.0"))
	float MaxPoise = 50.0f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Ironclad|Stagger")
	float CurrentPoise = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ironclad|Stagger", meta = (ClampMin = "0.0"))
	float PoiseRegenPerSecond = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ironclad|Stagger", meta = (ClampMin = "0.0"))
	float PoiseRegenDelaySeconds = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ironclad|Stagger", meta = (ClampMin = "0.0"))
	float StaggerDurationSeconds = 0.6f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Ironclad|Stagger")
	bool bIsStaggered = false;

	FTimerHandle PoiseRegenDelayTimer;
	FTimerHandle StaggerTimer;

	void DelayPoiseRegen();
	void BeginPoiseRegen();
	void TickPoiseRegen();
	void EndStagger();

	float ComputePoiseDamage(const FIroncladDamageSpec& Spec, float AppliedDamage) const;
	bool ApplyPoiseDamage(float PoiseDamage);


	// Hook already exists in your base; we will implement it
	virtual void OnDamageApplied(const FIroncladDamageSpec& Spec, const FIroncladDamageResult& Result);

	// Helpers
	void TryPlayHitReaction(const FIroncladDamageSpec& Spec, float AppliedDamage);
	void TryPlayDeath();

	void SetReactingLocked();
	void ClearReactingLocked();

	void ResetStaggerWindow();

	EIroncladHitReactionKind ClassifyReactionKind(const FIroncladDamageSpec& Spec) const;
	UAnimMontage* SelectReactionMontage(EIroncladHitReactionKind Kind) const;
};
