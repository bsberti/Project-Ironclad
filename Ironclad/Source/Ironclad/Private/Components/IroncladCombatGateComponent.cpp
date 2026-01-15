
#include "Components/IroncladCombatGateComponent.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY_STATIC(LogIroncladCombatGate, Log, All);

UIroncladCombatGateComponent::UIroncladCombatGateComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UIroncladCombatGateComponent::BeginPlay()
{
    Super::BeginPlay();

    CachedVitals = FindVitals();

#if !UE_BUILD_SHIPPING
    if (!CachedVitals)
    {
        UE_LOG(LogTemp, Warning, TEXT("[CombatGate] No IroncladVitalsComponent found on %s"), *GetOwner()->GetName());
    }
#endif
}

void UIroncladCombatGateComponent::DebugForceIdle()
{
#if !UE_BUILD_SHIPPING
    CombatState = ECombatState::Idle;
#endif
}

bool UIroncladCombatGateComponent::RequestLightAttack()
{
    return TryAcceptAction(ECombatAction::LightAttack, LightAttackStaminaCost, ECombatState::Attacking, TEXT("LightAttack"));
}

bool UIroncladCombatGateComponent::RequestHeavyAttack()
{
    return TryAcceptAction(ECombatAction::HeavyAttack, HeavyAttackStaminaCost, ECombatState::Attacking, TEXT("HeavyAttack"));
}

bool UIroncladCombatGateComponent::RequestDodge()
{
    return TryAcceptAction(ECombatAction::Dodge, DodgeStaminaCost, ECombatState::Dodging, TEXT("Dodge"));
}

void UIroncladCombatGateComponent::SetCombatState(ECombatState NewState)
{
    CombatState = NewState;
}

bool UIroncladCombatGateComponent::TryAcceptAction(ECombatAction Action, float StaminaCost, ECombatState StateToEnter, const TCHAR* DebugLabel)
{
    const float Now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;

    // Anti “double accept” for rapid input spam.
    if ((Now - LastAcceptedTime) < MinTimeBetweenAcceptedActions)
    {
#if !UE_BUILD_SHIPPING
        DebugReject(DebugLabel, TEXT("Rejected: MinTimeBetweenAcceptedActions"));
#endif
        return false;
    }

    if (!IsActionAllowedByState(Action))
    {
#if !UE_BUILD_SHIPPING
        DebugReject(DebugLabel, TEXT("Rejected: Invalid by state"));
#endif
        return false;
    }

    if (!HasSufficientStamina(StaminaCost))
    {
#if !UE_BUILD_SHIPPING
        DebugReject(DebugLabel, TEXT("Rejected: Insufficient stamina"));
#endif
        return false;
    }

    const ECombatState FromState = CombatState;

    // Commit: consume cost + transition state
    ConsumeStamina(StaminaCost);
    CombatState = StateToEnter;
    LastAcceptedTime = Now;

#if !UE_BUILD_SHIPPING
    DebugAccept(DebugLabel, Action, FromState, CombatState);
#endif

    OnCombatActionAccepted.Broadcast(Action, CombatState);
    return true;
}

bool UIroncladCombatGateComponent::IsActionAllowedByState(ECombatAction Action) const
{
    // Minimal, conservative gating rules:
    switch (CombatState)
    {
    case ECombatState::Idle:
        return true;

    case ECombatState::Recovering:
        // Start strict. Later you can allow dodge-cancel or buffered attacks explicitly.
        return false;

    case ECombatState::Attacking:
        // Prevent dodge while locked in attack for now (matches your card goal).
        // Later cards can add “cancel windows” via explicit flags.
        return false;

    case ECombatState::Dodging:
        return false;

    case ECombatState::Stunned:
        return false;

    default:
        return false;
    }
}

bool UIroncladCombatGateComponent::HasSufficientStamina(float Cost) const
{
    if (Cost <= 0.f) return true;

    // Placeholder: wire to your actual stamina source.
    // If no vitals are present yet, fail safe (reject).
    const UIroncladVitalsComponent* Vitals = FindVitals();
    if (!Vitals) return false;

    return Vitals->CanSpendStamina(Cost);
}

void UIroncladCombatGateComponent::ConsumeStamina(float Cost)
{
    if (Cost <= 0.f) return;

    UIroncladVitalsComponent* Vitals = FindVitals();
    if (!Vitals) return;

    Vitals->SpendStamina(Cost);
}

UIroncladVitalsComponent* UIroncladCombatGateComponent::FindVitals() const
{
    if (!GetOwner())
    {
        return nullptr;
    }

	UE_LOG(LogIroncladCombatGate, Log, TEXT("[CombatGate] Finding Vitals on %s"), *GetOwner()->GetName());

    // Prefer cached if already set (safe even if called before BeginPlay)
    if (CachedVitals)
    {
        return CachedVitals;
    }

    return GetOwner()->FindComponentByClass<UIroncladVitalsComponent>();
}

#if !UE_BUILD_SHIPPING
void UIroncladCombatGateComponent::DebugReject(const TCHAR* Label, const TCHAR* Reason) const
{
    UE_LOG(LogIroncladCombatGate, Log, TEXT("[CombatGate][%s] %s | State=%d"), Label, Reason, (int32)CombatState);
}

void UIroncladCombatGateComponent::DebugAccept(const TCHAR* Label, ECombatAction Action, ECombatState From, ECombatState To) const
{
    UE_LOG(LogIroncladCombatGate, Log, TEXT("[CombatGate][%s] Accepted Action=%d | %d -> %d"), Label, (int32)Action, (int32)From, (int32)To);
}
#endif
