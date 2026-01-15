#pragma once

#include "Components/IroncladVitalsComponent.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IroncladCombatGateComponent.generated.h"

UENUM(BlueprintType)
enum class ECombatState : uint8
{
    Idle,
    Attacking,
    Dodging,
    Recovering,
    Stunned,
};

UENUM(BlueprintType)
enum class ECombatAction : uint8
{
    LightAttack,
    HeavyAttack,
    Dodge,
};

USTRUCT(BlueprintType)
struct FCombatActionRequest
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    ECombatAction Action = ECombatAction::LightAttack;

    // Optional: track where it came from (input buffering, AI, etc.)
    UPROPERTY(BlueprintReadOnly)
    float Timestamp = 0.f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCombatActionAccepted, ECombatAction, Action, ECombatState, NewState);

UCLASS(ClassGroup = (Ironclad), meta = (BlueprintSpawnableComponent))
class IRONCLAD_API UIroncladCombatGateComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UIroncladCombatGateComponent();

    // Requests (called by character input or AI)
    UFUNCTION(BlueprintCallable)
    bool RequestLightAttack();

    UFUNCTION(BlueprintCallable)
    bool RequestHeavyAttack();

    UFUNCTION(BlueprintCallable)
    bool RequestDodge();

    // State controls (called by “executor” later: montages / anim notifies)
    UFUNCTION(BlueprintCallable)
    void SetCombatState(ECombatState NewState);

    UFUNCTION(BlueprintPure)
    ECombatState GetCombatState() const { return CombatState; }

    UFUNCTION(BlueprintCallable)
    void DebugForceIdle();

    // Lightweight event hook for next cards
    UPROPERTY(BlueprintAssignable)
    FOnCombatActionAccepted OnCombatActionAccepted;

    // Tuning
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Costs")
    float LightAttackStaminaCost = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Costs")
    float HeavyAttackStaminaCost = 20.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Costs")
    float DodgeStaminaCost = 15.f;

    // Optional: simple lockout window (prevents rapid double-accept)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Gating")
    float MinTimeBetweenAcceptedActions = 0.05f;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere, Category = "Combat")
    ECombatState CombatState = ECombatState::Idle;

    UPROPERTY(VisibleAnywhere, Category = "Combat")
    UIroncladVitalsComponent* CachedVitals = nullptr;

    float LastAcceptedTime = -FLT_MAX;

    bool TryAcceptAction(ECombatAction Action, float StaminaCost, ECombatState StateToEnter, const TCHAR* DebugLabel);

    bool IsActionAllowedByState(ECombatAction Action) const;
    bool HasSufficientStamina(float Cost) const;

    void ConsumeStamina(float Cost);

    // Replace these with your actual stamina component access pattern.
    class UIroncladVitalsComponent* FindVitals() const;

#if !UE_BUILD_SHIPPING
    void DebugReject(const TCHAR* Label, const TCHAR* Reason) const;
    void DebugAccept(const TCHAR* Label, ECombatAction Action, ECombatState From, ECombatState To) const;
#endif
};
