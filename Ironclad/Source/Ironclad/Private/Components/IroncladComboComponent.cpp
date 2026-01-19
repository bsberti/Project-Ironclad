#include "Components/IroncladComboComponent.h"

#include "Combat/IroncladComboDataAsset.h"
#include "GameFramework/Actor.h"
#include "Characters/IroncladPlayerCharacter.h"

DEFINE_LOG_CATEGORY(LogIroncladCombo);

static FString EnumToString_Action(ECombatAction Action)
{
    return UEnum::GetValueAsString(Action);
}

static FString EnumToString_State(ECombatState State)
{
    return UEnum::GetValueAsString(State);
}

UIroncladComboComponent::UIroncladComboComponent()
{
    PrimaryComponentTick.bCanEverTick = false; // first pass: no ticking needed
}

void UIroncladComboComponent::BeginPlay()
{
    Super::BeginPlay();
    OwnerCharacter = Cast<AIroncladPlayerCharacter>(GetOwner());
    if (OwnerCharacter.IsValid())
    {
        CombatGate = OwnerCharacter->FindComponentByClass<UIroncladCombatGateComponent>();
        if (CombatGate.IsValid())
        {
            CombatGate->OnCombatStateChanged.AddDynamic(
                this,
                &UIroncladComboComponent::OnCombatStateChanged
            );
        }
    }
}

void UIroncladComboComponent::OnCombatStateChanged(ECombatState FromState, ECombatState ToState)
{
    UE_LOG(LogIroncladCombo, Log,
        TEXT("CombatStateChanged %s -> %s"),
        *EnumToString_State(FromState),
        *EnumToString_State(ToState)
    );

    // Reset combo when combat ends
    if (ToState == ECombatState::Idle)
    {
        ResetCombo(FName(TEXT("ReturnedToIdle")));
        return;
    }

    // Reset on hard interruption
    if (ToState == ECombatState::Dodging || ToState == ECombatState::Stunned)
    {
        ResetCombo(FName(TEXT("CombatInterrupted")));
        return;
    }
}


void UIroncladComboComponent::RequestAction(ECombatAction Action)
{
    const double Now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0;

    if (!OwnerCharacter.IsValid())
    {
        return;
    }

    // Respect combat state gating (Stunned/Dodging should not accept attacks)
    if (CombatGate.IsValid())
    {
        const ECombatState State = CombatGate->GetCombatState(); // ensure you have a getter; if not, add one
        if (State == ECombatState::Stunned || State == ECombatState::Dodging)
        {
            if (bEnableComboDebugLogs)
            {
                UE_LOG(LogIroncladCombo, Log, TEXT("RequestAction rejected (%s) due to combat state %s."),
                    *EnumToString_Action(Action), *EnumToString_State(State));
            }
            return;
        }
    }

    if (bEnableComboDebugLogs)
    {
        UE_LOG(LogIroncladCombo, Log, TEXT("RequestAction: %s (CurrentStep=%d Window=%s)"),
            *EnumToString_Action(Action),
            CurrentStepIndex,
            bChainWindowOpen ? TEXT("OPEN") : TEXT("CLOSED"));
    }

    // If idle (no active step), start a combo
    if (CurrentStepIndex == INDEX_NONE)
    {
        (void)TryStartAtAction(Action, FName(TEXT("ComboStart")));
        return;
    }

    // Mid-combo: chain if window is open, otherwise buffer
    if (bChainWindowOpen)
    {
        if (!TryChain(Action, FName(TEXT("ComboChain"))))
        {
            // If chain rejected, you may choose to buffer (first pass: do NOT buffer rejected chains)
        }
    }
    else
    {
        BufferAction(Action, Now, FName(TEXT("ComboBuffered")));
    }
}

void UIroncladComboComponent::OpenChainWindow(FName DebugTag)
{
    bChainWindowOpen = true;

    if (bEnableComboDebugLogs)
    {
        UE_LOG(LogIroncladCombo, Log, TEXT("ChainWindow OPEN (Step=%d Tag=%s)"),
            CurrentStepIndex, *DebugTag.ToString());
    }

    (void)ConsumeBufferedIfPossible(DebugTag.IsNone() ? FName(TEXT("ConsumeBuffered")) : DebugTag);
}

void UIroncladComboComponent::CloseChainWindow(FName DebugTag)
{
    bChainWindowOpen = false;

    if (bEnableComboDebugLogs)
    {
        UE_LOG(LogIroncladCombo, Log, TEXT("ChainWindow CLOSE (Step=%d Tag=%s)"),
            CurrentStepIndex, *DebugTag.ToString());
    }
}

void UIroncladComboComponent::OnCombatStateChanged(ECombatState Prev, ECombatState Next, FName DebugTag)
{
    if (bEnableComboDebugLogs)
    {
        UE_LOG(LogIroncladCombo, Log, TEXT("CombatStateChanged %s -> %s (Tag=%s)"),
            *EnumToString_State(Prev), *EnumToString_State(Next), *DebugTag.ToString());
    }

    // Reset combo whenever combat fully ends
    if (Next == ECombatState::Idle)
    {
        ResetCombo(FName(TEXT("ReturnedToIdle")));
        return;
    }

    // Reset immediately on interruptions
    if (Next == ECombatState::Dodging || Next == ECombatState::Stunned)
    {
        ResetCombo(FName(TEXT("CombatStateInterrupt")));
    }
}

void UIroncladComboComponent::OnAttackMontageEnded(bool bInterrupted, FName DebugTag)
{
    // If interrupted, reset deterministically (first pass rule)
    if (bInterrupted)
    {
        if (bEnableComboDebugLogs)
        {
            UE_LOG(LogIroncladCombo, Log, TEXT("AttackMontageEnded: interrupted -> reset (Step=%d Tag=%s)"),
                CurrentStepIndex, *DebugTag.ToString());
        }

        ResetCombo(FName(TEXT("MontageInterrupted")));
        return;
    }

    // Not interrupted: allow timeout-based reset if player doesn't continue.
    // First pass: we reset immediately if there is no buffered action and window is closed.
    // If you want a more permissive feel, keep the current step until timeout expires.
    const double Now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0;

    const FIroncladComboStep* Step = ComboData ? ComboData->GetStep(CurrentStepIndex) : nullptr;
    const float Timeout = Step ? Step->ResetTimeoutSeconds : 1.0f;

    if (Timeout <= 0.0f)
    {
        ResetCombo(FName(TEXT("MontageEnded")));
        return;
    }

    // If there is buffered input, we expect chaining to happen on the next window open; do not reset here.
    if (bHasBufferedAction && !IsBufferedExpired(Now))
    {
        return;
    }

    // If montage ended and we didn't chain, reset now (simple deterministic first pass)
    ResetCombo(FName(TEXT("MontageEndedNoChain")));
}

bool UIroncladComboComponent::TryStartAtAction(ECombatAction Action, FName DebugTag)
{
    if (!ComboData || ComboData->Steps.Num() == 0)
    {
        UE_LOG(LogIroncladCombo, Warning, TEXT("TryStartAtAction failed: ComboData is null or empty."));
        return false;
    }

    // Find the first step with matching Input (deterministic by array order)
    for (int32 i = 0; i < ComboData->Steps.Num(); ++i)
    {
        if (ComboData->Steps[i].Input == Action)
        {
            return StartStep(i, DebugTag);
        }
    }

    if (bEnableComboDebugLogs)
    {
        UE_LOG(LogIroncladCombo, Log, TEXT("TryStartAtAction: no starting step for %s"), *EnumToString_Action(Action));
    }
    return false;
}

bool UIroncladComboComponent::TryChain(ECombatAction Action, FName DebugTag)
{
    if (!ComboData)
    {
        return false;
    }

    const FIroncladComboStep* Step = ComboData->GetStep(CurrentStepIndex);
    if (!Step)
    {
        ResetCombo(FName(TEXT("InvalidCurrentStep")));
        return false;
    }

    const int32 NextIndex = GetNextStepIndex(*Step, Action);
    if (NextIndex == INDEX_NONE)
    {
        if (bEnableComboDebugLogs)
        {
            UE_LOG(LogIroncladCombo, Log, TEXT("ChainRejected: no next step from %d on %s"),
                CurrentStepIndex, *EnumToString_Action(Action));
        }
        return false;
    }

    if (bEnableComboDebugLogs)
    {
        UE_LOG(LogIroncladCombo, Log, TEXT("ChainAccepted: %d -> %d via %s (Tag=%s)"),
            CurrentStepIndex, NextIndex, *EnumToString_Action(Action), *DebugTag.ToString());
    }

    return StartStep(NextIndex, DebugTag);
}

bool UIroncladComboComponent::StartStep(int32 StepIndex, FName DebugTag)
{
    if (!OwnerCharacter.IsValid() || !ComboData)
    {
        return false;
    }

    const FIroncladComboStep* Step = ComboData->GetStep(StepIndex);
    if (!Step || !Step->Montage)
    {
        UE_LOG(LogIroncladCombo, Warning, TEXT("StartStep failed: invalid step or montage (Index=%d)."), StepIndex);
        return false;
    }

    // Map ECombatAction -> EAttackKind
    const EAttackKind AttackKind =
        (Step->Input == ECombatAction::HeavyAttack) ? EAttackKind::Heavy : EAttackKind::Light;

    // Play through your centralized function (with optional section support)
    const bool bPlayed = OwnerCharacter->TryStartComboAttackMontage(
        AttackKind,
        Step->Montage,
        Step->RecoverySeconds,
        DebugTag.IsNone() ? Step->DebugStepName : DebugTag,
        Step->SectionName
    );

    if (!bPlayed)
    {
        if (bEnableComboDebugLogs)
        {
            UE_LOG(LogIroncladCombo, Log, TEXT("StartStep failed to play montage (Index=%d)."), StepIndex);
        }
        return false;
    }

    CurrentStepIndex = StepIndex;
    bChainWindowOpen = false;

    bHasBufferedAction = false; // consuming/chaining clears buffer deterministically
    BufferedAtSeconds = 0.0;

    StepStartedAtSeconds = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0;

    if (bEnableComboDebugLogs)
    {
        UE_LOG(LogIroncladCombo, Log, TEXT("StartStep: Index=%d Name=%s Montage=%s Section=%s Recovery=%.2f"),
            StepIndex,
            *Step->DebugStepName.ToString(),
            *GetNameSafe(Step->Montage),
            *Step->SectionName.ToString(),
            Step->RecoverySeconds);
    }

    return true;
}

int32 UIroncladComboComponent::GetNextStepIndex(const FIroncladComboStep& Step, ECombatAction Action) const
{
    switch (Action)
    {
    case ECombatAction::LightAttack: return Step.NextOnLight;
    case ECombatAction::HeavyAttack: return Step.NextOnHeavy;
    default: return INDEX_NONE;
    }
}

bool UIroncladComboComponent::IsBufferedExpired(double NowSeconds) const
{
    return !bHasBufferedAction || ((NowSeconds - BufferedAtSeconds) > BufferExpireSeconds);
}

void UIroncladComboComponent::BufferAction(ECombatAction Action, double NowSeconds, FName DebugTag)
{
    bHasBufferedAction = true;
    BufferedAction = Action;
    BufferedAtSeconds = NowSeconds;

    if (bEnableComboDebugLogs)
    {
        UE_LOG(LogIroncladCombo, Log, TEXT("BufferedAction: %s (Step=%d Window=CLOSED Tag=%s)"),
            *EnumToString_Action(Action), CurrentStepIndex, *DebugTag.ToString());
    }
}

bool UIroncladComboComponent::ConsumeBufferedIfPossible(FName DebugTag)
{
    const double Now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0;

    if (!bHasBufferedAction || IsBufferedExpired(Now))
    {
        if (bHasBufferedAction && bEnableComboDebugLogs)
        {
            UE_LOG(LogIroncladCombo, Log, TEXT("BufferedAction expired; clearing."));
        }
        bHasBufferedAction = false;
        return false;
    }

    const ECombatAction Action = BufferedAction;
    bHasBufferedAction = false;

    if (bEnableComboDebugLogs)
    {
        UE_LOG(LogIroncladCombo, Log, TEXT("ConsumeBufferedAction: %s (Tag=%s)"),
            *EnumToString_Action(Action), *DebugTag.ToString());
    }

    return TryChain(Action, DebugTag);
}

void UIroncladComboComponent::ResetCombo(FName Reason)
{
    if (bEnableComboDebugLogs)
    {
        UE_LOG(LogIroncladCombo, Log, TEXT("ResetCombo: Reason=%s PrevStep=%d"), *Reason.ToString(), CurrentStepIndex);
    }

    CurrentStepIndex = INDEX_NONE;
    bChainWindowOpen = false;

    bHasBufferedAction = false;
    BufferedAtSeconds = 0.0;

    StepStartedAtSeconds = 0.0;
}
