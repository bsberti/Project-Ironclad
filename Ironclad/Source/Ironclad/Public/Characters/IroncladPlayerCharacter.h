#pragma once

#include "Components/IroncladCombatGateComponent.h"
#include "Components/IroncladWeaponComponent.h"
#include "Components/IroncladComboComponent.h"
#include "Components/IroncladAbilityComponent.h"

#include "Abilities/IroncladAbilityDataAsset.h"

#include "Animation/AnimMontage.h"

#include "Combat/IroncladCombatTuningDataAsset.h"

#include "CoreMinimal.h"
#include "IroncladCharacterBase.h"
#include "InputActionValue.h"
#include "IroncladPlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UIroncladCombatGateComponent; 
class UIroncladWeaponComponent;

UENUM(BlueprintType)
enum class EAttackKind : uint8
{
    Light,
    Heavy,
    // Future: ComboStep, AbilityAttack, etc.
};

USTRUCT(BlueprintType)
struct FAttackExecutionContext
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    EAttackKind AttackKind = EAttackKind::Light;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UAnimMontage> Montage = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float RecoverySeconds = 0.0f;

    // Optional, but helpful for logs / profiling / state reasons
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FName DebugTag = NAME_None;

    // True once we begin recovery so BlendOut + End can't double-trigger
    bool bRecoveryStarted = false;

    void Reset()
    {
        AttackKind = EAttackKind::Light;
        Montage = nullptr;
        RecoverySeconds = 0.0f;
        DebugTag = NAME_None;
        bRecoveryStarted = false;
    }

    bool IsValid() const { return Montage != nullptr; }
};

UCLASS()
class IRONCLAD_API AIroncladPlayerCharacter : public AIroncladCharacterBase
{
    GENERATED_BODY()

public:
    AIroncladPlayerCharacter();

    UFUNCTION(BlueprintPure, Category = "Movement|Sprint")
    bool IsSprinting() const { return bIsSprinting; }

    // Hit window toggles (stubbed for now)
    void SetHitWindowActive(bool bActive);

    bool TryStartComboAttackMontage(
        EAttackKind AttackKind,
        UAnimMontage* Montage,
        float RecoverySeconds,
        FName DebugTag,
        FName SectionName = NAME_None
    );

    UIroncladComboComponent* GetComboComponent() const { return ComboComponent; }

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

	// ----------------- INPUT ACTIONS ------------------

    UPROPERTY(EditDefaultsOnly, Category = "Input|Controls")
    UInputAction* MoveAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input|Controls")
    UInputAction* LookAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input|Controls")
    UInputAction* JumpAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Debug")
    class UInputAction* DebugDamageAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Controls")
    class UInputAction* SprintAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Controls")
    class UInputAction* LockOnAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Combat")
    class UInputAction* LightAttackAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Combat")
    class UInputAction* HeavyAttackAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Combat")
    class UInputAction* DodgeAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Debug")
    class UInputAction* DebugForceIdleStateAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Debug")
    class UInputAction* DebugMakeNoiseAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Combat")
    class UInputAction* ChangeWeaponAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Ability")
    class UInputAction* StaminaBurstAction;

    // --------------------------------------------------
    // ------------------ COMPONENTS --------------------

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class UCameraComponent* FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    UIroncladCombatGateComponent* CombatGate;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Combo", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UIroncladComboComponent> ComboComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    UIroncladWeaponComponent* WeaponComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
    UIroncladAbilityComponent* AbilityComponent;

    // --------------------------------------------------
    // --------------- ACTION FUNCTIONS -----------------

    UFUNCTION()
    void ToggleLockOnDebug();

    UFUNCTION()
    void DebugApplyDamage();

    UFUNCTION()
    void StartSprint();

    UFUNCTION()
    void ToggleLockOn();

    UFUNCTION()
    void StopSprint();

    UFUNCTION()
    void OnLightAttackPressed();

    UFUNCTION()
    void OnHeavyAttackPressed();

    UFUNCTION()
    void OnDodgePressed();

    UFUNCTION()
    void DebugForceIdleCombatState();

    UFUNCTION()
    void CycleWeapon();

    UFUNCTION()
    void OnStaminaBurstPressed();

    UFUNCTION()
    void DebugMakeNoise();

    // --------------------------------------------------

    UPROPERTY(EditDefaultsOnly, Category = "Abilities")
    TObjectPtr<UIroncladAbilityDataAsset> StaminaBurstAbility = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Sprint")
    float WalkSpeed = 450.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Sprint")
    float SprintSpeed = 750.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Sprint")
    float SprintStaminaCostPerSecond = 15.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement|Sprint")
    bool bIsSprinting = false;

    // --- Generic attack lifecycle state ---
    UPROPERTY(VisibleAnywhere, Category = "Combat|Attack")
    FAttackExecutionContext ActiveAttack;

    FTimerHandle AttackRecoveryTimerHandle;

    UPROPERTY(EditDefaultsOnly, Category = "Combat|Attack")
    bool bReturnToIdleOnBlendOut = true;

    UPROPERTY(EditDefaultsOnly, Category = "Combat|Tuning")
    TObjectPtr<UIroncladCombatTuningDataAsset> CombatTuning = nullptr;

    // Generic callbacks (ONE set for all attacks)
    UFUNCTION()
    void OnAttackMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

    UFUNCTION()
    void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

    void BeginAttackRecovery();
    void FinishAttackRecovery();

    // Starts any attack in a single unified way
    bool StartAttackMontage(
        EAttackKind AttackKind,
        UAnimMontage* Montage,
        float RecoverySeconds,
        FName DebugTag,
        FName SectionName = NAME_None
    );

    virtual void HandleDeath() override;

    virtual void Tick(float DeltaSeconds) override;
    
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "LockOn")
    bool bIsLockedOn = false;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "LockOn")
    TObjectPtr<AActor> LockedTarget = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Tuning")
    float LockOnRotationInterpSpeed = 12.0f;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Tuning")
    float LockOnMaxDistance = 1500.0f;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Tuning")
    bool bLockPitchToTarget = false;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Targeting")
    float TargetSearchRadius = 2000.f;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Targeting")
    float MaxTargetAngleDegrees = 60.f; // cone around camera forward

    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Targeting")
    bool bRequireLineOfSight = true;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Targeting")
    TSubclassOf<AActor> LockOnTargetClass; // set to your Enemy base BP/C++ class when available

    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Targeting")
    FName LockOnTargetTag = "LockOnTarget"; // fallback if class not set

    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Camera")
    float LockOnArmLength = 320.f;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Camera")
    FVector LockOnSocketOffset = FVector(0.f, 60.f, 60.f); // right + up offset

    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Camera")
    float CameraInterpSpeed = 8.f;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Validation")
    float AutoUnlockDistance = 1600.f;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Validation")
    bool bAutoUnlockOnLineOfSightLost = true;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Validation", meta = (EditCondition = "bAutoUnlockOnLineOfSightLost"))
    float LineOfSightGraceSeconds = 0.35f;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Validation")
    bool bAutoUnlockWhenOutsideCone = false;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Validation", meta = (EditCondition = "bAutoUnlockWhenOutsideCone"))
    float OutsideConeGraceSeconds = 0.50f;

    // ----- Internal helpers -----
    void EnableLockOn(AActor* NewTarget);
    void DisableLockOn();
    bool IsTargetValid(AActor* Target) const;

    AActor* FindBestLockOnTarget();

    bool HasLineOfSightToTarget(const AActor* Target) const;

    float DefaultArmLength = 0.f;
    FVector DefaultSocketOffset = FVector::ZeroVector;

    float TimeWithoutLineOfSight = 0.f;
    float TimeOutsideCone = 0.f;

    bool IsLockOnStillValid(float DeltaSeconds);

    // Debug toggles
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<class UInputAction> ToggleLockOnDebugAction = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Debug")
    bool bDebugLockOn = false;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Debug")
    bool bDebugDrawSearch = true;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Debug")
    bool bDebugDrawLOS = true;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Debug")
    bool bDebugPrintOnScreen = true;

    void DrawLockOnOnScreenDebug() const;
    void DrawLockOnWorldDebug() const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Dodge")
    TObjectPtr<UAnimMontage> DodgeMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Dodge")
    float DodgeImpulseStrength = 900.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Dodge")
    float DodgeCooldownSeconds = 0.35f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Dodge|IFrames")
    float IFrameStartSeconds = 0.02f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Dodge|IFrames")
    float IFrameEndSeconds = 0.14f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Dodge")
    float DodgeRecoverySeconds = 0.55f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Dodge")
    float DodgePlayRate = 2.5f;

private:
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

    void StartJump();
    void StopJump();

    // Recovery timer handle
    FTimerHandle LightAttackRecoveryTimerHandle;

    UFUNCTION()
    void HandleCombatActionAccepted(ECombatAction Action, ECombatState NewState);

    void BeginDodge();
    void EndDodge(const TCHAR* Reason);

    FVector ComputeDodgeDirection() const;

    void SetInvulnerable(bool bEnable);

    // Cached flag (stub)
    bool bIsHitWindowActive = false;

    bool bIsInvulnerable = false;
    float LastDodgeTime = -1000.f;

    FTimerHandle Timer_IFrameOn;
    FTimerHandle Timer_IFrameOff;
    FTimerHandle Timer_DodgeRecovery;

    FOnMontageEnded DodgeMontageEndedDelegate;

    float SavedBrakingFrictionFactor = 0.f;
    float SavedGroundFriction = 0.f;
    float SavedBrakingDecel = 0.f;

    UFUNCTION()
    void OnDodgeMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
