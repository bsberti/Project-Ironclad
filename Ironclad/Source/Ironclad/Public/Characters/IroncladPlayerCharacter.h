#pragma once

#include "Components/IroncladCombatGateComponent.h"
#include "Components/IroncladWeaponComponent.h"

#include "Animation/AnimMontage.h"

#include "CoreMinimal.h"
#include "IroncladCharacterBase.h"
#include "InputActionValue.h"
#include "IroncladPlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UIroncladCombatGateComponent; 
class UIroncladWeaponComponent;


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

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

	// --- Input Actions -----

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* LookAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class UCameraComponent* FollowCamera;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    class UInputAction* DebugDamageAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    class UInputAction* SprintAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    class UInputAction* LockOnAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    class UInputAction* LightAttackAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    class UInputAction* HeavyAttackAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    class UInputAction* DodgeAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    class UInputAction* DebugForceIdleStateAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    class UInputAction* ChangeWeaponAction;

    // --- Input Actions -----

    UFUNCTION()
    void DebugApplyDamage();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Sprint")
    float WalkSpeed = 450.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Sprint")
    float SprintSpeed = 750.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Sprint")
    float SprintStaminaCostPerSecond = 15.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement|Sprint")
    bool bIsSprinting = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    UIroncladCombatGateComponent* CombatGate;

    UPROPERTY(EditDefaultsOnly, Category = "Combat|Light Attack")
    TObjectPtr<UAnimMontage> LightAttackMontage = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Combat|Light Attack")
    float LightAttackPlayRate = 1.0f;

    // Recovery duration after montage completes (seconds)
    UPROPERTY(EditDefaultsOnly, Category = "Combat|Light Attack")
    float LightAttackRecoverySeconds = 0.35f;

    // Optional: if you want to auto-return to locomotion even when montage blends out
    UPROPERTY(EditDefaultsOnly, Category = "Combat|Light Attack")
    bool bReturnToIdleOnBlendOut = true;

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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    UIroncladWeaponComponent* WeaponComponent;

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

    // Allow enabling debug via input
    UFUNCTION()
    void ToggleLockOnDebug();

    void DrawLockOnOnScreenDebug() const;
    void DrawLockOnWorldDebug() const;

private:
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

    void StartJump();
    void StopJump();

    UFUNCTION()
    void OnLightAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

    UFUNCTION()
    void OnLightAttackMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

    // Recovery timer handle
    FTimerHandle LightAttackRecoveryTimerHandle;

    void BeginLightAttackRecovery();
    void FinishLightAttackRecovery();

    // Cached flag (stub)
    bool bIsHitWindowActive = false;
};
