#pragma once

#include "CoreMinimal.h"
#include "IroncladCharacterBase.h"
#include "InputActionValue.h"
#include "IroncladPlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class IRONCLAD_API AIroncladPlayerCharacter : public AIroncladCharacterBase
{
    GENERATED_BODY()

public:
    AIroncladPlayerCharacter();

    UFUNCTION(BlueprintPure, Category = "Movement|Sprint")
    bool IsSprinting() const { return bIsSprinting; }

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* LookAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* JumpAction;

    /** Camera boom positioning the camera behind the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class USpringArmComponent* CameraBoom;

    /** Follow camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class UCameraComponent* FollowCamera;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    class UInputAction* DebugDamageAction;

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    class UInputAction* SprintAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    class UInputAction* LockOnAction;

    UFUNCTION()
    void StartSprint();

    UFUNCTION()
    void ToggleLockOn();

    UFUNCTION()
    void StopSprint();

    virtual void Tick(float DeltaSeconds) override;
    // ----- Lock-on state -----
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "LockOn")
    bool bIsLockedOn = false;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "LockOn")
    TObjectPtr<AActor> LockedTarget = nullptr;

    // ----- Tuning -----
    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Tuning")
    float LockOnRotationInterpSpeed = 12.0f;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Tuning")
    float LockOnMaxDistance = 2000.0f;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Tuning")
    bool bLockPitchToTarget = false;

    // Lock-on targeting filters
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

    // Lock-on camera tuning
    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Camera")
    float LockOnArmLength = 320.f;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Camera")
    FVector LockOnSocketOffset = FVector(0.f, 60.f, 60.f); // right + up offset

    UPROPERTY(EditDefaultsOnly, Category = "LockOn|Camera")
    float CameraInterpSpeed = 8.f;

    // ----- Internal helpers -----
    void EnableLockOn(AActor* NewTarget);
    void DisableLockOn();
    bool IsTargetValid(AActor* Target) const;

    AActor* FindBestLockOnTarget();

    bool HasLineOfSightToTarget(const AActor* Target) const;

    float DefaultArmLength = 0.f;
    FVector DefaultSocketOffset = FVector::ZeroVector;

private:
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

    void StartJump();
    void StopJump();
};
