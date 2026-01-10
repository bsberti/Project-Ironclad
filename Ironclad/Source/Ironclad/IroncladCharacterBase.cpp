#include "IroncladCharacterBase.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AIroncladCharacterBase::AIroncladCharacterBase()
{
    PrimaryActorTick.bCanEverTick = true;

    // Disable direct controller rotation on the character
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Character rotates toward movement direction (template-like default)
    if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
    {
        MoveComp->bOrientRotationToMovement = true;
        MoveComp->RotationRate = FRotator(0.f, 540.f, 0.f);
    }

    // Camera Boom (Spring Arm)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 350.f;            // Third-person distance
    CameraBoom->bUsePawnControlRotation = true;     // Rotate arm based on controller

    // Follow Camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;  // Camera does not rotate relative to arm
}

void AIroncladCharacterBase::BeginPlay()
{
    Super::BeginPlay();
}

void AIroncladCharacterBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
