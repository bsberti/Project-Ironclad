#include "Combat/Damage/IroncladDamageReceiverComponent.h"
#include "Components/IroncladVitalsComponent.h"
#include "Characters/IroncladCharacterBase.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY(LogIroncladDamage);

UIroncladDamageReceiverComponent::UIroncladDamageReceiverComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UIroncladDamageReceiverComponent::BeginPlay()
{
	Super::BeginPlay();
}

FIroncladDamageResult UIroncladDamageReceiverComponent::ApplyDamage_Implementation(const FIroncladDamageSpec& Spec)
{
    FIroncladDamageResult Result;
    Result.bApplied = false;
    Result.AppliedAmount = 0.0f;
    Result.Reason = FName(TEXT("Unknown"));

    AIroncladCharacterBase* OwnerChar = Cast<AIroncladCharacterBase>(GetOwner());
    if (!OwnerChar)
    {
        Result.Reason = FName(TEXT("OwnerNotCharacterBase"));
        return Result;
    }

    UIroncladVitalsComponent* Vitals = OwnerChar->GetVitals(); // if you have a getter; otherwise access member
    if (!Vitals)
    {
        Result.Reason = FName(TEXT("NoVitals"));
        return Result;
    }

    if (Vitals->IsDead())
    {
        Result.Reason = FName(TEXT("Dead"));
        return Result;
    }

    const float Amount = FMath::Max(0.0f, Spec.FinalAmount);
    if (Amount <= 0.0f)
    {
        Result.Reason = FName(TEXT("ZeroDamage"));
        return Result;
    }

    const float Old = Vitals->GetHealth(); // add getter if missing
    const bool bApplied = Vitals->ApplyDamage(Amount);
    const float NewHealth = Vitals->GetHealth();

    Result.bApplied = bApplied;
    Result.AppliedAmount = bApplied ? (Old - NewHealth) : 0.0f;
    Result.Reason = bApplied ? NAME_None : FName(TEXT("Rejected"));

    if (bLogDamage)
    {
        UE_LOG(LogIroncladDamage, Display,
            TEXT("[Damage] %s took %.2f damage (%.2f -> %.2f). Type=%d Tags=%s From=%s"),
            *GetNameSafe(GetOwner()),
            Result.AppliedAmount,
            Old,
            NewHealth,
            (int32)Spec.DamageType,
            *Spec.Tags.ToStringSimple(),
            *GetNameSafe(Spec.SourceActor)
        );
    }

    return Result;
}
