#include "IroncladPlayerCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"

AIroncladPlayerCharacter::AIroncladPlayerCharacter()
{
    // Nothing required here yet
}

void AIroncladPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Add the mapping context to the local player subsystem
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (ULocalPlayer* LP = PC->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
                LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                if (DefaultMappingContext) {
                    UE_LOG(LogTemp, Warning, TEXT("Adding IMC: %s"), *DefaultMappingContext->GetName());
                    Subsystem->AddMappingContext(DefaultMappingContext, 0);
                }
                else {
                    UE_LOG(LogTemp, Warning, TEXT("DefaultMappingContext is not set on %s"), *GetName());
                }
            }
        }
    }
}

void AIroncladPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (!EnhancedInput)
    {
        return;
    }

    if (MoveAction) {
        EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AIroncladPlayerCharacter::Move);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("MoveAction is not set on %s"), *GetName());
	}

    if (LookAction) {
        UE_LOG(LogTemp, Warning, TEXT("Binding LookAction: %s"), *LookAction->GetName());
        EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AIroncladPlayerCharacter::Look);
    }
    else {
		UE_LOG(LogTemp, Warning, TEXT("LookAction is not set on %s"), *GetName());
    }

    if (JumpAction) {
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &AIroncladPlayerCharacter::StartJump);
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &AIroncladPlayerCharacter::StopJump);
    } else {
		UE_LOG(LogTemp, Warning, TEXT("JumpAction is not set on %s"), *GetName());
	}
}

void AIroncladPlayerCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D MoveValue = Value.Get<FVector2D>();
    if (Controller == nullptr) return;

    const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    AddMovementInput(ForwardDirection, MoveValue.Y);
    AddMovementInput(RightDirection, MoveValue.X);
}

void AIroncladPlayerCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D LookValue = Value.Get<FVector2D>();
    UE_LOG(LogTemp, Warning, TEXT("Look() X=%f Y=%f (Pawn=%s)"), LookValue.X, LookValue.Y, *GetName());

    AddControllerYawInput(LookValue.X);
    AddControllerPitchInput(LookValue.Y);
}

void AIroncladPlayerCharacter::StartJump()
{
    Jump();
}

void AIroncladPlayerCharacter::StopJump()
{
    StopJumping();
}
