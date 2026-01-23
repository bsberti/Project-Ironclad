#include "Characters/IroncladCharacterBase.h"

#include "Animation/AnimInstance.h"
#include "TimerManager.h"

#include "Combat/Damage/IroncladDamageReceiverComponent.h"

#include "Components/IroncladVitalsComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AIroncladCharacterBase::AIroncladCharacterBase()
{
    PrimaryActorTick.bCanEverTick = true;

    VitalsComponent = CreateDefaultSubobject<UIroncladVitalsComponent>(TEXT("VitalsComponent"));
    DamageReceiver = CreateDefaultSubobject<UIroncladDamageReceiverComponent>(TEXT("DamageReceiver"));

	CurrentPoise = MaxPoise;
}

FIroncladDamageResult AIroncladCharacterBase::ApplyDamage_Implementation(const FIroncladDamageSpec& Spec)
{
    UE_LOG(LogIroncladDamage, Log,
        TEXT("[Damage] %s::ApplyDamage_Implementation called on %s"),
        *GetClass()->GetName(),
        *GetNameSafe(this));

    FIroncladDamageResult Result;
    Result.bApplied = false;
    Result.Reason = FName(TEXT("Unknown"));

    if (!DamageReceiver)
    {
        Result.bApplied = false;
        Result.Reason = FName(TEXT("NoDamageReceiver"));
        return Result;
    }

    Result = DamageReceiver->ApplyDamage_Implementation(Spec);

    OnDamageApplied(Spec, Result);

	UE_LOG(LogIroncladDamage, Log,
		TEXT("[Damage] %s::ApplyDamage_Implementation result: bDeathHandled=%s Result.bApplied=%s VitalsComponent->IsDead()=%s"),
		*GetClass()->GetName(),
		bDeathHandled ? TEXT("true") : TEXT("false"),
		Result.bApplied ? TEXT("true") : TEXT("false"),
		VitalsComponent && VitalsComponent->IsDead() ? TEXT("true") : TEXT("false"));

    if (VitalsComponent && Result.bApplied && VitalsComponent->IsDead()) {
        HandleDeath();
    }

    return Result;
}

void AIroncladCharacterBase::OnDamageApplied(const FIroncladDamageSpec& Spec, const FIroncladDamageResult& Result)
{
    // Only react when damage was actually applied
    if (!Result.bApplied)
    {
        return;
    }

    // If already dead (or death already handled), do not play hit reacts
    if (bDeathHandled || IsDead())
    {
        return;
    }

    // If your Result has an applied amount, use it. If not, use Spec.Amount.
    // Adjust these fields if your struct names differ.
    const float AppliedDamage = (Result.AppliedAmount > 0.f) ? Result.AppliedAmount : Spec.FinalAmount;

    if (AppliedDamage <= 0.f)
    {
        return;
    }

    TryPlayHitReaction(Spec, AppliedDamage);
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

void AIroncladCharacterBase::HandleDeath()
{
    if (bDeathHandled) {
        return;
    }

    bDeathHandled = true;

    if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
    {
        MoveComp->DisableMovement();
        MoveComp->StopMovementImmediately();
    }

    SetActorEnableCollision(false);

    // Play death montage (if assigned)
    TryPlayDeath();
}

void AIroncladCharacterBase::BeginPlay()
{
    Super::BeginPlay();

	CurrentPoise = MaxPoise;

    if (VitalsComponent)
    {
        VitalsComponent->OnDeath.AddDynamic(this, &AIroncladCharacterBase::HandleDeath);
    }
}

void AIroncladCharacterBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AIroncladCharacterBase::TryPlayHitReaction(const FIroncladDamageSpec& Spec, float AppliedDamage)
{
	UE_LOG(LogIroncladDamage, Log,
		TEXT("[Damage] %s::TryPlayHitReaction called on %s"),
		*GetClass()->GetName(),
		*GetNameSafe(this));

	if (!bAllowReactWhileReacting && bIsReacting)
	{
		return;
	}

	EIroncladHitReactionKind Kind = ClassifyReactionKind(Spec);

	// Refined stagger: poise break
	if (bUsePoise)
	{
		const float PoiseDamage = ComputePoiseDamage(Spec, AppliedDamage);
		const bool bTriggeredStagger = ApplyPoiseDamage(PoiseDamage);
		if (bTriggeredStagger)
		{
			Kind = EIroncladHitReactionKind::Stagger;
		}
	}

	UAnimMontage* MontageToPlay = SelectReactionMontage(Kind);
	UE_LOG(LogIroncladDamage, Warning,
		TEXT("[Damage] TryPlayHitReaction: Kind=%d Montage=%s"),
		(int32)Kind,
		*GetNameSafe(MontageToPlay)
	);

	if (!MontageToPlay)
	{
		return; // No montage assigned; silent fail is fine for now
	}

	USkeletalMeshComponent* MeshComp = GetMesh();
	UE_LOG(LogIroncladDamage, Warning,
		TEXT("[Damage] TryPlayHitReaction: Mesh=%s AnimClass=%s AnimInst=%s"),
		*GetNameSafe(MeshComp),
		MeshComp ? *GetNameSafe(MeshComp->GetAnimClass()) : TEXT("None"),
		MeshComp && MeshComp->GetAnimInstance() ? *GetNameSafe(MeshComp->GetAnimInstance()) : TEXT("None")
	);

	if (!MontageToPlay || !MeshComp || !MeshComp->GetAnimInstance())
	{
		UE_LOG(LogIroncladDamage, Error, TEXT("[Damage] TryPlayHitReaction: Abort (missing montage/mesh/animinstance)"));
		return;
	}

	UAnimInstance* AnimInst = MeshComp->GetAnimInstance();
	const float PlayResult = AnimInst->Montage_Play(MontageToPlay, 1.0f);

	UE_LOG(LogIroncladDamage, Warning,
		TEXT("[Damage] TryPlayHitReaction: Montage_Play returned %0.3f"),
		PlayResult
	);

	if (PlayResult > 0.f)
	{
		SetReactingLocked();
	}
}

void AIroncladCharacterBase::TryPlayDeath()
{
	UE_LOG(LogIroncladDamage, Log,
		TEXT("[Damage] %s::TryPlayDeath called on %s"),
		*GetClass()->GetName(),
		*GetNameSafe(this));

	if (!DeathMontage) {
		return;
	}

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (UAnimInstance* AnimInst = MeshComp->GetAnimInstance())
		{
			AnimInst->Montage_Play(DeathMontage);
		}
	}
}

void AIroncladCharacterBase::SetReactingLocked()
{
	bIsReacting = true;

	if (ReactionLockSeconds <= 0.f)
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(ReactionLockTimer);
	GetWorldTimerManager().SetTimer(ReactionLockTimer, this, &AIroncladCharacterBase::ClearReactingLocked, ReactionLockSeconds, false);
}

void AIroncladCharacterBase::ClearReactingLocked()
{
	bIsReacting = false;
}

void AIroncladCharacterBase::ResetStaggerWindow()
{
	AccumulatedDamageInWindow = 0.f;
}

EIroncladHitReactionKind AIroncladCharacterBase::ClassifyReactionKind(const FIroncladDamageSpec& Spec) const
{
	// Explicit tags first (most authoritative)
	if (Spec.Tags.HasTagExact(FGameplayTag::RequestGameplayTag(TEXT("Attack.Stagger"))))
	{
		return EIroncladHitReactionKind::Stagger;
	}

	if (Spec.Tags.HasTagExact(FGameplayTag::RequestGameplayTag(TEXT("Attack.Heavy"))))
	{
		return EIroncladHitReactionKind::Heavy;
	}

	if (Spec.Tags.HasTagExact(FGameplayTag::RequestGameplayTag(TEXT("Attack.Light"))))
	{
		return EIroncladHitReactionKind::Light;
	}

	// Fallbacks: infer from damage magnitude (optional first pass)
	if (Spec.FinalAmount >= 30.f)
	{
		return EIroncladHitReactionKind::Heavy;
	}

	return EIroncladHitReactionKind::Light;
}

UAnimMontage* AIroncladCharacterBase::SelectReactionMontage(EIroncladHitReactionKind Kind) const
{
	switch (Kind)
	{
	case EIroncladHitReactionKind::Stagger:
		// Use heavy montage for stagger first pass, fallback to light
		return HitReactHeavyMontage ? HitReactHeavyMontage : HitReactLightMontage;

	case EIroncladHitReactionKind::Heavy:
		return HitReactHeavyMontage ? HitReactHeavyMontage : HitReactLightMontage;

	case EIroncladHitReactionKind::Light:
	default:
		return HitReactLightMontage;
	}
}

float AIroncladCharacterBase::ComputePoiseDamage(const FIroncladDamageSpec& Spec, float AppliedDamage) const
{
	// Default: poise damage roughly tracks damage
	float PoiseDamage = AppliedDamage;

	// Simple semantic modifiers (first pass)
	// Add these tags in your attack specs when creating damage.
	const FGameplayTag TagHeavy = FGameplayTag::RequestGameplayTag(TEXT("Attack.Heavy"));
	const FGameplayTag TagLight = FGameplayTag::RequestGameplayTag(TEXT("Attack.Light"));
	const FGameplayTag TagStagger = FGameplayTag::RequestGameplayTag(TEXT("Attack.Stagger"));

	if (Spec.Tags.HasTag(TagStagger))
	{
		PoiseDamage *= 2.5f;
	}
	else if (Spec.Tags.HasTag(TagHeavy))
	{
		PoiseDamage *= 1.75f;
	}
	else if (Spec.Tags.HasTag(TagLight))
	{
		PoiseDamage *= 1.0f;
	}

	return FMath::Max(0.f, PoiseDamage);
}

bool AIroncladCharacterBase::ApplyPoiseDamage(float PoiseDamage)
{
	if (!bUsePoise || MaxPoise <= 0.f || bIsStaggered)
	{
		return false;
	}

	DelayPoiseRegen();

	CurrentPoise = FMath::Clamp(CurrentPoise - PoiseDamage, 0.f, MaxPoise);
	UE_LOG(LogIroncladDamage, Warning,
		TEXT("[Damage] ApplyPoiseDamage: PoiseDamage=%0.3f CurrentPoise=%0.3f / %0.3f"),
		PoiseDamage,
		CurrentPoise,
		MaxPoise
	);

	if (CurrentPoise <= 0.f)
	{
		bIsStaggered = true;

		// Reset poise immediately so it can regen after stagger ends
		CurrentPoise = 0.f;

		GetWorldTimerManager().ClearTimer(StaggerTimer);
		GetWorldTimerManager().SetTimer(StaggerTimer, this, &AIroncladCharacterBase::EndStagger, StaggerDurationSeconds, false);

		if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
		{
			MoveComp->StopMovementImmediately();
		}

		return true; // stagger triggered
	}

	return false;
}

void AIroncladCharacterBase::DelayPoiseRegen()
{
	// Stop ongoing regen tick if any
	GetWorldTimerManager().ClearTimer(PoiseRegenDelayTimer);

	if (PoiseRegenDelaySeconds <= 0.f)
	{
		BeginPoiseRegen();
		return;
	}

	GetWorldTimerManager().SetTimer(PoiseRegenDelayTimer, this, &AIroncladCharacterBase::BeginPoiseRegen, PoiseRegenDelaySeconds, false);
}

void AIroncladCharacterBase::BeginPoiseRegen()
{
	// Tick regen at a modest rate (10Hz)
	GetWorldTimerManager().SetTimer(PoiseRegenDelayTimer, this, &AIroncladCharacterBase::TickPoiseRegen, 0.1f, true);
}

void AIroncladCharacterBase::TickPoiseRegen()
{
	if (bIsStaggered)
	{
		return; // wait until stagger ends
	}

	if (CurrentPoise >= MaxPoise)
	{
		CurrentPoise = MaxPoise;
		GetWorldTimerManager().ClearTimer(PoiseRegenDelayTimer);
		return;
	}

	const float Delta = PoiseRegenPerSecond * 0.1f;
	CurrentPoise = FMath::Min(MaxPoise, CurrentPoise + Delta);
}

void AIroncladCharacterBase::EndStagger()
{
	bIsStaggered = false;

	// Give a small buffer so you don’t instantly re-break in the same frame window
	DelayPoiseRegen();
}
