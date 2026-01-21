#include "Combat/Damage/IroncladDamageReceiverComponent.h"

#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY(LogIroncladDamage);

UIroncladDamageReceiverComponent::UIroncladDamageReceiverComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UIroncladDamageReceiverComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);
	if (CurrentHealth <= 0.0f)
	{
		CurrentHealth = MaxHealth;
	}
}

FIroncladDamageResult UIroncladDamageReceiverComponent::ApplyDamage_Implementation(const FIroncladDamageSpec& Spec)
{
	FIroncladDamageResult Result;

	if (IsDead())
	{
		Result.bApplied = false;
		Result.AppliedAmount = 0.0f;
		Result.Reason = FName(TEXT("Dead"));
		return Result;
	}

	const float Amount = FMath::Max(0.0f, Spec.FinalAmount);

	if (Amount <= 0.0f)
	{
		Result.bApplied = false;
		Result.AppliedAmount = 0.0f;
		Result.Reason = FName(TEXT("ZeroDamage"));
		return Result;
	}

	const float Old = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth - Amount, 0.0f, MaxHealth);

	Result.bApplied = true;
	Result.AppliedAmount = (Old - CurrentHealth);
	Result.Reason = NAME_None;

	if (bLogDamage)
	{
		UE_LOG(LogIroncladDamage, Display,
			TEXT("[Damage] %s took %.2f damage (%.2f -> %.2f). Type=%d Tags=%s From=%s"),
			*GetNameSafe(GetOwner()),
			Result.AppliedAmount,
			Old,
			CurrentHealth,
			(int32)Spec.DamageType,
			*Spec.Tags.ToStringSimple(),
			*GetNameSafe(Spec.SourceActor)
		);
	}

	return Result;
}
