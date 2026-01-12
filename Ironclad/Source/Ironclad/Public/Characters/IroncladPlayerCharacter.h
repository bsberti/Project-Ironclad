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

    UFUNCTION()
    void StartSprint();

    UFUNCTION()
    void StopSprint();

    virtual void Tick(float DeltaSeconds) override;


private:
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

    void StartJump();
    void StopJump();
};
