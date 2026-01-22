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

    if (!bDeathHandled && Result.bApplied && DamageReceiver->IsDead())
    {
        bDeathHandled = true;
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

	// Stagger first pass: accumulate damage in a small window
	if (bEnableStagger && StaggerDamageThreshold > 0.f)
	{
		AccumulatedDamageInWindow += AppliedDamage;

		// Start/reset the stagger window timer
		if (StaggerWindowSeconds > 0.f)
		{
			GetWorldTimerManager().ClearTimer(StaggerWindowTimer);
			GetWorldTimerManager().SetTimer(StaggerWindowTimer, this, &AIroncladCharacterBase::ResetStaggerWindow, StaggerWindowSeconds, false);
		}

		if (AccumulatedDamageInWindow >= StaggerDamageThreshold)
		{
			Kind = EIroncladHitReactionKind::Stagger;
			AccumulatedDamageInWindow = 0.f;
			GetWorldTimerManager().ClearTimer(StaggerWindowTimer);
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

	if (MeshComp)
	{
		if (AnimInst)
		{
			AnimInst->Montage_Play(MontageToPlay);
			SetReactingLocked();
		}
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
