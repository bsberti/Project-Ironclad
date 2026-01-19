#pragma once

#include "Combat/IroncladCombatTypes.h"
#include "Components/IroncladCombatGateComponent.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IroncladComboComponent.generated.h"

class UIroncladComboDataAsset;
class AIroncladPlayerCharacter;

DECLARE_LOG_CATEGORY_EXTERN(LogIroncladCombo, Log, All);

UCLASS(ClassGroup = (Ironclad), meta = (BlueprintSpawnableComponent))
class IRONCLAD_API UIroncladComboComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UIroncladComboComponent();

    UFUNCTION()
    void OnCombatStateChanged(ECombatState FromState, ECombatState ToState);

    // Assign in BP_IroncladPlayerCharacter (or defaults)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
    TObjectPtr<UIroncladComboDataAsset> ComboData = nullptr;

    TWeakObjectPtr<UIroncladCombatGateComponent> CombatGate;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
    float BufferExpireSeconds = 0.50f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
    bool bEnableComboDebugLogs = true;

    // Input entry point (from Light/Heavy)
    void RequestAction(ECombatAction Action);

    // Called by AnimNotifyState
    void OpenChainWindow(FName DebugTag = NAME_None);
    void CloseChainWindow(FName DebugTag = NAME_None);

    // Hook from your combat state changes (recommended)
    void OnCombatStateChanged(ECombatState Prev, ECombatState Next, FName DebugTag);

    // Hook from your montage callbacks
    void OnAttackMontageEnded(bool bInterrupted, FName DebugTag);

    // Hard reset
    void ResetCombo(FName Reason);

    // Useful for logs/diagnostics
    int32 GetCurrentStepIndex() const { return CurrentStepIndex; }

private:
    virtual void BeginPlay() override;

    bool TryStartAtAction(ECombatAction Action, FName DebugTag);
    bool TryChain(ECombatAction Action, FName DebugTag);
    bool StartStep(int32 StepIndex, FName DebugTag);

    int32 GetNextStepIndex(const struct FIroncladComboStep& Step, ECombatAction Action) const;

    bool IsBufferedExpired(double NowSeconds) const;
    void BufferAction(ECombatAction Action, double NowSeconds, FName DebugTag);
    bool ConsumeBufferedIfPossible(FName DebugTag);

private:
    TWeakObjectPtr<AIroncladPlayerCharacter> OwnerCharacter;

    int32 CurrentStepIndex = INDEX_NONE;
    bool bChainWindowOpen = false;

    bool bHasBufferedAction = false;
    ECombatAction BufferedAction = ECombatAction::LightAttack;
    double BufferedAtSeconds = 0.0;

    double StepStartedAtSeconds = 0.0;
};
