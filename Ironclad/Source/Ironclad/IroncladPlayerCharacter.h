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

private:
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

    void StartJump();
    void StopJump();
};
