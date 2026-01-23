#include "Components/IroncladAbilityComponent.h"

#include "Abilities/IroncladAbilityDataAsset.h"
#include "Characters/IroncladCharacterBase.h"
#include "Components/IroncladVitalsComponent.h"
#include "Combat/Damage/IroncladDamageable.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

UIroncladAbilityComponent::UIroncladAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UIroncladAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerChar = Cast<AIroncladCharacterBase>(GetOwner());
}

FIroncladAbilityActivationResult UIroncladAbilityComponent::TryActivateAbility(UIroncladAbilityDataAsset* Ability, AActor* OptionalTarget)
{
	FIroncladAbilityActivationResult Result;

	if (!Ability || Ability->AbilityId.IsNone())
	{
		Result.FailReason = FName(TEXT("InvalidAbility"));
		DebugPrint(Ability, Result);
		return Result;
	}

	FName FailReason = NAME_None;
	if (!CanActivate(Ability, FailReason))
	{
		Result.FailReason = FailReason;
		DebugPrint(Ability, Result);
		return Result;
	}

	Commit(Ability);

	// If montage-driven, play montage and wait for notify execution.
	if (Ability->ActivationMontage && Ability->ExecutionMode == EIroncladAbilityExecutionMode::MontageNotify)
	{
		if (OwnerChar.IsValid() && OwnerChar->GetMesh() && OwnerChar->GetMesh()->GetAnimInstance())
		{
			OwnerChar->GetMesh()->GetAnimInstance()->Montage_Play(Ability->ActivationMontage, 1.0f);

			PendingAbility = Ability;
			PendingTarget = OptionalTarget;

			Result.bActivated = true;
			DebugPrint(Ability, Result);
			return Result;
		}

		Result.FailReason = FName(TEXT("NoAnimInstance"));
		DebugPrint(Ability, Result);
		return Result;
	}

	// Instant execution (with or without montage)
	if (Ability->ActivationMontage)
	{
		if (OwnerChar.IsValid() && OwnerChar->GetMesh() && OwnerChar->GetMesh()->GetAnimInstance())
		{
			OwnerChar->GetMesh()->GetAnimInstance()->Montage_Play(Ability->ActivationMontage, 1.0f);
		}
	}

	Execute(Ability, OptionalTarget);

	Result.bActivated = true;
	DebugPrint(Ability, Result);
	return Result;
}

bool UIroncladAbilityComponent::CanActivate(UIroncladAbilityDataAsset* Ability, FName& OutFailReason) const
{
	if (!OwnerChar.IsValid())
	{
		OutFailReason = FName(TEXT("NoOwner"));
		return false;
	}

	// Hard gates
	if (OwnerChar->IsDead())
	{
		OutFailReason = FName(TEXT("Dead"));
		return false;
	}

	// If you exposed bIsStaggered in CharacterBase, gate here.
	// (If not exposed yet, skip for now.)
	// if (OwnerChar->IsStaggered()) { OutFailReason = "Staggered"; return false; }

	// Cooldown gate
	const double Now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0;
	const double* NextReady = NextReadyTimeByAbility.Find(Ability->AbilityId);
	if (NextReady && Now < *NextReady)
	{
		OutFailReason = FName(TEXT("Cooldown"));
		return false;
	}

	// Cost gate
	UIroncladVitalsComponent* Vitals = OwnerChar->GetVitals();
	if (!Vitals)
	{
		OutFailReason = FName(TEXT("NoVitals"));
		return false;
	}

	if (Ability->StaminaCost > 0.f)
	{
		// If you have a CanSpendStamina, use it. If not, do a conservative check if you expose current stamina.
		const bool bSpent = Vitals->CanSpendStamina(Ability->StaminaCost);
		if (!bSpent)
		{
			OutFailReason = FName(TEXT("NoStamina"));
			return false;
		}
	}

	return true;
}

void UIroncladAbilityComponent::Commit(UIroncladAbilityDataAsset* Ability)
{
	if (!OwnerChar.IsValid())
	{
		return;
	}

	UIroncladVitalsComponent* Vitals = OwnerChar->GetVitals();
	if (Vitals && Ability->StaminaCost > 0.f)
	{
		(void)Vitals->SpendStamina(Ability->StaminaCost);
	}

	const double Now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0;
	if (Ability->CooldownSeconds > 0.f)
	{
		NextReadyTimeByAbility.Add(Ability->AbilityId, Now + Ability->CooldownSeconds);
	}
}

void UIroncladAbilityComponent::Execute(UIroncladAbilityDataAsset* Ability, AActor* OptionalTarget)
{
	if (!OwnerChar.IsValid() || !Ability)
	{
		return;
	}

	UIroncladVitalsComponent* Vitals = OwnerChar->GetVitals();

	switch (Ability->EffectType)
	{
	case EIroncladAbilityEffectType::StaminaBurst:
		if (Vitals)
		{
			Vitals->RestoreStamina(Ability->StaminaRestoreAmount); // add if missing
		}
		break;

	case EIroncladAbilityEffectType::ApplyDamage:
		if (OptionalTarget && OptionalTarget->GetClass()->ImplementsInterface(UIroncladDamageable::StaticClass()))
		{
			FIroncladDamageSpec Spec = Ability->DamageTemplate;
			Spec.SourceActor = OwnerChar.Get();
			Spec.InstigatorActor = OwnerChar.Get();
			Spec.Finalize();

			IIroncladDamageable::Execute_ApplyDamage(OptionalTarget, Spec);
		}
		break;

	default:
		break;
	}
}

void UIroncladAbilityComponent::ExecutePendingAbility()
{
	if (!PendingAbility)
	{
		return;
	}

	Execute(PendingAbility, PendingTarget);

	PendingAbility = nullptr;
	PendingTarget = nullptr;
}

float UIroncladAbilityComponent::GetRemainingCooldown(const UIroncladAbilityDataAsset* Ability) const
{
	if (!Ability || Ability->AbilityId.IsNone())
	{
		return 0.f;
	}

	const double Now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0;
	const double* NextReady = NextReadyTimeByAbility.Find(Ability->AbilityId);
	if (!NextReady)
	{
		return 0.f;
	}

	return (float)FMath::Max(0.0, (*NextReady - Now));
}

void UIroncladAbilityComponent::DebugPrint(UIroncladAbilityDataAsset* Ability, const FIroncladAbilityActivationResult& Result) const
{
	if (!bEnableAbilityDebug)
	{
		return;
	}

	// Special case: cooldown with remaining time
	if (!Result.bActivated && Result.FailReason == FName(TEXT("Cooldown")))
	{
		const float Remaining = GetRemainingCooldown(Ability);
		const FString Msg = FString::Printf(TEXT("[Ability] %s -> Rejected (Cooldown %.2fs)"),
			*Ability->AbilityId.ToString(),
			Remaining);

		UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, Msg);
		}
		return;
	}

	// Default path for all other results
	const FString Msg = FString::Printf(TEXT("[Ability] %s -> %s (%s)"),
		*Ability->AbilityId.ToString(),
		Result.bActivated ? TEXT("Activated") : TEXT("Rejected"),
		*Result.FailReason.ToString());

	UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, Msg);
	}
}
