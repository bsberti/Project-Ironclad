#include "Characters/IroncladCharacterBase.h"

#include "Combat/Damage/IroncladDamageReceiverComponent.h"

#include "Components/IroncladVitalsComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AIroncladCharacterBase::AIroncladCharacterBase()
{
    PrimaryActorTick.bCanEverTick = true;

    VitalsComponent = CreateDefaultSubobject<UIroncladVitalsComponent>(TEXT("VitalsComponent"));
    DamageReceiver = CreateDefaultSubobject<UIroncladDamageReceiverComponent>(TEXT("DamageReceiver"));
}

FIroncladDamageResult AIroncladCharacterBase::ApplyDamage_Implementation(const FIroncladDamageSpec& Spec)
{
    if (DamageReceiver)
    {
        return DamageReceiver->ApplyDamage_Implementation(Spec);
    }

    FIroncladDamageResult Result;
    Result.bApplied = false;
    Result.Reason = FName(TEXT("NoDamageReceiver"));
    return Result;
}

bool AIroncladCharacterBase::ApplyDamageToVitals(float Amount)
{
    return VitalsComponent ? VitalsComponent->ApplyDamage(Amount) : false;
}

bool AIroncladCharacterBase::HealVitals(float Amount)
{
    return VitalsComponent ? VitalsComponent->Heal(Amount) : false;
}

bool AIroncladCharacterBase::SpendStamina(float Amount)
{
    return VitalsComponent ? VitalsComponent->SpendStamina(Amount) : false;
}

bool AIroncladCharacterBase::IsDead() const
{
    return VitalsComponent ? VitalsComponent->IsDead() : false;
}

float AIroncladCharacterBase::TakeDamage(
    float DamageAmount,
    FDamageEvent const& DamageEvent,
    AController* EventInstigator,
    AActor* DamageCauser
)
{
    const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    UE_LOG(LogTemp, Warning, TEXT("TakeDamage() DamageAmount=%f )"), DamageAmount);

    if (!VitalsComponent || ActualDamage <= 0.f)
    {
        return 0.f;
    }

    const bool bApplied = VitalsComponent->ApplyDamage(ActualDamage);
    return bApplied ? ActualDamage : 0.f;
}

void AIroncladCharacterBase::HandleDeath()
{
    // Minimal Phase 1 behavior: stop movement and disable collisions
    if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
    {
        MoveComp->DisableMovement();
    }

    SetActorEnableCollision(false);
}

void AIroncladCharacterBase::BeginPlay()
{
    Super::BeginPlay();

    if (VitalsComponent)
    {
        VitalsComponent->OnDeath.AddDynamic(this, &AIroncladCharacterBase::HandleDeath);
    }
}

void AIroncladCharacterBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
