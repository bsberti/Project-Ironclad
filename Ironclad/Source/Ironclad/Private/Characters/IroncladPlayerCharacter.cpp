#include "Characters/IroncladPlayerCharacter.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Camera/CameraComponent.h"

#include "Engine/DamageEvents.h"

#include "Components/IroncladVitalsComponent.h"

#include "Kismet/KismetMathLibrary.h"

AIroncladPlayerCharacter::AIroncladPlayerCharacter()
{
    // Character does NOT rotate directly with controller; camera does.
    bUseControllerRotationYaw = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;

    // Movement-driven facing (GoW-ish baseline)
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

    // --- Camera (player-only) ---
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.f;
    CameraBoom->SocketOffset = FVector(0.f, 50.f, 60.f);

    // This is the critical line for pitch/yaw camera control
    CameraBoom->bUsePawnControlRotation = true;

    // Follow camera attached to boom
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    PrimaryActorTick.bCanEverTick = true;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

}

void AIroncladPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC) {
        return;
    }

    // Optional: Align camera yaw to actor yaw on start
    PC->SetControlRotation(FRotator(0.f, GetActorRotation().Yaw, 0.f));

    // Limit vertical look (cinematic pitch clamp)
    if (PC->PlayerCameraManager)
    {
        PC->PlayerCameraManager->ViewPitchMin = -45.f;
        PC->PlayerCameraManager->ViewPitchMax = 30.f;
    }

    // Add the mapping context to the local player subsystem
    if (ULocalPlayer* LP = PC->GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (DefaultMappingContext) 
            {
                UE_LOG(LogTemp, Warning, TEXT("Adding IMC: %s"), *DefaultMappingContext->GetName());
                Subsystem->AddMappingContext(DefaultMappingContext, 0);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("DefaultMappingContext is not set on %s"), *GetName());
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

    if (DebugDamageAction) {
        EnhancedInput->BindAction(DebugDamageAction, ETriggerEvent::Started, this, &AIroncladPlayerCharacter::DebugApplyDamage);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("DebugDamageAction is not set on %s"), *GetName());
    }

    if (SprintAction) {
        EnhancedInput->BindAction(SprintAction, ETriggerEvent::Started, this, &AIroncladPlayerCharacter::StartSprint);
        EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &AIroncladPlayerCharacter::StopSprint);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("SprintAction is not set on %s"), *GetName());
    }

    if (LockOnAction) {
        EnhancedInput->BindAction(LockOnAction, ETriggerEvent::Started, this, &AIroncladPlayerCharacter::ToggleLockOn);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("LockOnAction is not set on %s"), *GetName());
    }
}

void AIroncladPlayerCharacter::ToggleLockOn() 
{
    UE_LOG(LogTemp, Warning, TEXT("ToggleLockOn fired. Pawn=%s Controller=%s"),
        *GetNameSafe(this),
        *GetNameSafe(Controller));

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1, 2.0f, FColor::Yellow,
            TEXT("ToggleLockOn fired")
        );
    }

    // If currently locked, toggle off.
    if (bIsLockedOn)
    {
        DisableLockOn();
        return;
    }

    // Otherwise attempt to lock onto something.
    AActor* Candidate = FindBestLockOnTarget(); // Step 3 will implement.

    if (!Candidate)
    {
        // No target found -> remain unlocked. Keep this log for early testing.
        UE_LOG(LogTemp, Verbose, TEXT("ToggleLockOn: no valid target found."));
        return;
    }

    EnableLockOn(Candidate);
}

void AIroncladPlayerCharacter::EnableLockOn(AActor* NewTarget)
{
    if (!IsTargetValid(NewTarget))
    {
        UE_LOG(LogTemp, Verbose, TEXT("EnableLockOn: target invalid."));
        return;
    }

    bIsLockedOn = true;
    LockedTarget = NewTarget;

    // Rotation behavior: lock-on wants character to follow controller yaw, and movement to strafe.
    bUseControllerRotationYaw = true;

    if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
    {
        MoveComp->bOrientRotationToMovement = false;
        // Optional: tweak rotation rate if you want snappier facing
        // MoveComp->RotationRate = FRotator(0.f, 720.f, 0.f);
    }

    // Immediately face the target on lock to avoid one-frame mismatch.
    if (Controller)
    {
        const FVector ToTarget = LockedTarget->GetActorLocation() - GetActorLocation();
        FRotator Desired = ToTarget.Rotation();
        if (!bLockPitchToTarget)
        {
            Desired.Pitch = Controller->GetControlRotation().Pitch;
        }
        Controller->SetControlRotation(Desired);
    }

    UE_LOG(LogTemp, Log, TEXT("LockOn: ENABLED -> %s"), *GetNameSafe(LockedTarget));
}


void AIroncladPlayerCharacter::DisableLockOn()
{
    bIsLockedOn = false;

    if (LockedTarget)
    {
        UE_LOG(LogTemp, Log, TEXT("LockOn: DISABLED (was %s)"), *GetNameSafe(LockedTarget));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("LockOn: DISABLED"));
    }

    LockedTarget = nullptr;

    // Restore your foundation movement/camera behavior.
    bUseControllerRotationYaw = false;

    if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
    {
        MoveComp->bOrientRotationToMovement = true;
    }
}

bool AIroncladPlayerCharacter::IsTargetValid(AActor* Target) const
{
    if (!Target || Target->IsPendingKillPending())
    {
        return false;
    }

    // Distance gate (prevents silly locks).
    const float DistSq = FVector::DistSquared(Target->GetActorLocation(), GetActorLocation());
    if (DistSq > FMath::Square(LockOnMaxDistance))
    {
        return false;
    }

    return true;
}

AActor* AIroncladPlayerCharacter::FindBestLockOnTarget()
{
    // Step 3 will implement target acquisition.
    // Returning nullptr keeps ToggleLockOn safe while you build acquisition logic.
    //EnableLockOn(this);
    return nullptr;
}

void AIroncladPlayerCharacter::StartSprint()
{
    if (IsDead())
    {
        return;
    }

    // Must have enough stamina to start sprinting
    if (!GetVitals() || GetVitals()->Stamina <= 0.f)
    {
        return;
    }

    bIsSprinting = true;
    GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AIroncladPlayerCharacter::StopSprint()
{
    bIsSprinting = false;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AIroncladPlayerCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!bIsSprinting || IsDead())
    {
        return;
    }

    if (bIsLockedOn)
    {
        if (!IsTargetValid(LockedTarget))
        {
            DisableLockOn();
        }
        else if (Controller)
        {
            const FVector ToTarget = LockedTarget->GetActorLocation() - GetActorLocation();
            FRotator Desired = ToTarget.Rotation();

            // Preserve pitch unless you explicitly want pitch lock
            if (!bLockPitchToTarget)
            {
                Desired.Pitch = Controller->GetControlRotation().Pitch;
            }

            const FRotator Current = Controller->GetControlRotation();
            const FRotator Smoothed = FMath::RInterpTo(
                Current,
                Desired,
                DeltaSeconds,
                LockOnRotationInterpSpeed
            );

            Controller->SetControlRotation(Smoothed);
        }
    }

    if (bIsSprinting)
    {
        // Only spend stamina if we're actually trying to move
        const FVector Velocity = GetVelocity();
        const bool bIsMoving = Velocity.SizeSquared2D() > 1.f;

        if (bIsMoving)
        {
            const float Cost = SprintStaminaCostPerSecond * DeltaSeconds;

            // If we can't spend, stop sprint
            if (!SpendStamina(Cost))
            {
                StopSprint();
            }
        }
    }
}


void AIroncladPlayerCharacter::DebugApplyDamage()
{
    // Apply damage via character TakeDamage pipeline
    UE_LOG(LogTemp, Warning, TEXT("DebugApplyDamage: Calling TakeDamage(25)"));
    TakeDamage(25.f, FDamageEvent(), GetController(), this);

    // Optional: Spend stamina to verify stamina path too
    if (GetVitals())
    {
        const bool bSpent = SpendStamina(20.f);
        UE_LOG(LogTemp, Warning, TEXT("DebugApplyDamage: SpendStamina(20) -> %s"), bSpent ? TEXT("true") : TEXT("false"));
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

    if (bIsLockedOn)
    {
        // Let Tick control yaw; optionally allow pitch adjustment.
        if (!bLockPitchToTarget)
        {
            AddControllerPitchInput(LookValue.Y);
        }
        return;
    }

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
