
#include "Components/IroncladVitalsComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UIroncladVitalsComponent::UIroncladVitalsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UIroncladVitalsComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = FMath::Clamp(Health, 0.f, MaxHealth);
	Stamina = FMath::Clamp(Stamina, 0.f, MaxStamina);

	BroadcastHealth();
	BroadcastStamina();	
}

void UIroncladVitalsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (Stamina >= MaxStamina)
    {
        SetComponentTickEnabled(false);
        return;
    }

    if (bIsDead || !bStaminaRegenEnabled)
    {
        return;
    }

    TimeSinceStaminaSpend += DeltaTime;
    if (TimeSinceStaminaSpend < StaminaRegenDelay)
    {
        return;
    }

    if (Stamina < MaxStamina)
    {
        const float Old = Stamina;
        Stamina = FMath::Min(MaxStamina, Stamina + (StaminaRegenRate * DeltaTime));
        if (!FMath::IsNearlyEqual(Old, Stamina))
        {
            BroadcastStamina();
        }
    }
}

bool UIroncladVitalsComponent::ApplyDamage(float Amount)
{
    if (bIsDead || Amount <= 0.f)
    {
        return false;
    }

    const float Old = Health;
    Health = FMath::Clamp(Health - Amount, 0.f, MaxHealth);

    if (!FMath::IsNearlyEqual(Old, Health))
    {
        BroadcastHealth();
    }

    if (Health <= 0.f && !bIsDead)
    {
        HandleDeath();
    }

    return true;
}

bool UIroncladVitalsComponent::Heal(float Amount)
{
    if (bIsDead || Amount <= 0.f)
    {
        return false;
    }

    const float Old = Health;
    Health = FMath::Clamp(Health + Amount, 0.f, MaxHealth);

    if (!FMath::IsNearlyEqual(Old, Health))
    {
        BroadcastHealth();
        return true;
    }

    return false;
}

bool UIroncladVitalsComponent::CanSpendStamina(float Amount) const
{
    return (!bIsDead && Amount > 0.f && Stamina >= Amount);
}

bool UIroncladVitalsComponent::SpendStamina(float Amount)
{
    if (CanSpendStamina(Amount))
    {
        const float Old = Stamina;
        Stamina = FMath::Clamp(Stamina - Amount, 0.f, MaxStamina);
        TimeSinceStaminaSpend = 0.f;

		//UE_LOG(LogTemp, Log, TEXT("SpendStamina: Spent %f stamina, now at %f/%f"), Amount, Stamina, MaxStamina);

        if (!FMath::IsNearlyEqual(Old, Stamina))
        {
            BroadcastStamina();
        }

        SetComponentTickEnabled(true);
        return true;
    }
    else
    {
        return false;
    }
}

bool UIroncladVitalsComponent::RestoreStamina(float Amount)
{
    if (bIsDead || Amount <= 0.f)
    {
        return false;
    }

    const float Old = Stamina;
    Stamina = FMath::Clamp(Stamina + Amount, 0.f, MaxStamina);

    if (!FMath::IsNearlyEqual(Old, Stamina))
    {
        BroadcastStamina();
    }

    return true;
}

float UIroncladVitalsComponent::GetHealthNormalized() const
{
    return (MaxHealth > 0.f) ? (Health / MaxHealth) : 0.f;
}

float UIroncladVitalsComponent::GetHealth() const
{
    return Health;
}

float UIroncladVitalsComponent::GetStaminaNormalized() const
{
    return (MaxStamina > 0.f) ? (Stamina / MaxStamina) : 0.f;
}

void UIroncladVitalsComponent::BroadcastHealth()
{
    OnHealthChanged.Broadcast(Health, MaxHealth);
}

void UIroncladVitalsComponent::BroadcastStamina()
{
    OnStaminaChanged.Broadcast(Stamina, MaxStamina);
}

void UIroncladVitalsComponent::HandleDeath()
{
    bIsDead = true;
    OnDeath.Broadcast();
}

void UIroncladVitalsComponent::InitializeVitals(float InMaxHealth)
{
    MaxHealth = FMath::Max(1.f, InMaxHealth);
    Health = MaxHealth;
}