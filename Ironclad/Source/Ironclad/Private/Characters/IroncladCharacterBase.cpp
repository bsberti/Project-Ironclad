#include "Characters/IroncladCharacterBase.h"

#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Animation/AnimInstance.h"

#include "UI/IroncladFloatingDamageActor.h"

#include "Kismet/GameplayStatics.h"

#include "Combat/Damage/IroncladDamageReceiverComponent.h"

#include "Components/IroncladVitalsComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

AIroncladCharacterBase::AIroncladCharacterBase()
{
    PrimaryActorTick.bCanEverTick = true;

    VitalsComponent = CreateDefaultSubobject<UIroncladVitalsComponent>(TEXT("VitalsComponent"));
    DamageReceiver = CreateDefaultSubobject<UIroncladDamageReceiverComponent>(TEXT("DamageReceiver")); 
	CombatGate = CreateDefaultSubobject<UIroncladCombatGateComponent>(TEXT("CombatGate"));

	CurrentPoise = MaxPoise;
}

void AIroncladCharacterBase::HandleDamageTaken(float DamageAmount)
{
	UE_LOG(LogIroncladDamage, Warning, TEXT("[Damage] HandleDamageTaken called: %s Damage=%.1f"), *GetNameSafe(this), DamageAmount);

	UWorld* World = VitalsComponent ? VitalsComponent->GetWorld() : GetWorld();

	if (DamageAmount <= 0.f || !World || !FloatingDamageActorClass)
	{
		UE_LOG(LogIroncladDamage, Warning,
			TEXT("[Damage] %s::HandleDamageTaken invalid params: DamageAmount=%.1f World=%s FloatingDamageActorClass=%s"),
			*GetClass()->GetName(),
			DamageAmount,
			World ? TEXT("valid") : TEXT("null"),
			*GetNameSafe(FloatingDamageActorClass));
		return;
	}

	const FVector SpawnLoc = GetActorLocation() + FVector(0.f, 0.f, 120.f);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* Spawned = World->SpawnActor<AActor>(FloatingDamageActorClass, SpawnLoc, FRotator::ZeroRotator, Params);

	UE_LOG(LogIroncladDamage, Warning, TEXT("[Damage] Spawned class=%s"), *GetNameSafe(Spawned ? Spawned->GetClass() : nullptr));
	UE_LOG(LogIroncladDamage, Warning, TEXT("[Damage] Spawned CDO=%s"), *GetNameSafe(Spawned ? Spawned->GetClass()->GetDefaultObject() : nullptr));

	if (AIroncladFloatingDamageActor* DamageActor = Cast<AIroncladFloatingDamageActor>(Spawned))
	{
		UE_LOG(LogIroncladDamage, Warning, TEXT("[Damage] DamageActor runtime class=%s"), *GetNameSafe(DamageActor->GetClass()));
		DamageActor->Init(DamageAmount);
	}
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

	SetActionLockForReaction(true);

    if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
    {
        MoveComp->DisableMovement();
        MoveComp->StopMovementImmediately();
    }

	// Stop AI logic (CRITICAL: prevents BT from calling attack tasks forever)
	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		AIC->StopMovement();
		AIC->ClearFocus(EAIFocusPriority::Gameplay);

		if (UBrainComponent* Brain = AIC->GetBrainComponent())
		{
			Brain->StopLogic(TEXT("Death"));
		}
	}

	// Stop any active montages before death montage (prevents “attack montage fighting death montage”)
	if (UAnimInstance* AnimInst = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr)
	{
		AnimInst->StopAllMontages(0.05f);
	}

    SetActorEnableCollision(false);

    // Play death montage (if assigned)
    TryPlayDeath();
}

void AIroncladCharacterBase::BeginPlay()
{
    Super::BeginPlay();

	UE_LOG(LogIroncladDamage, Warning, TEXT("[Damage] %s BeginPlay: FloatingDamageActorClass=%s"),
		*GetNameSafe(this),
		*GetNameSafe(FloatingDamageActorClass));

	CurrentPoise = MaxPoise;

    if (VitalsComponent)
    {
        VitalsComponent->OnDeath.AddDynamic(this, &AIroncladCharacterBase::HandleDeath);
		VitalsComponent->OnDamageTaken.AddDynamic(this, &AIroncladCharacterBase::HandleDamageTaken);
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

	bool bIsStagger = (Kind == EIroncladHitReactionKind::Stagger);

	// Refined stagger: poise break
	if (bUsePoise)
	{
		const float PoiseDamage = ComputePoiseDamage(Spec, AppliedDamage);
		const bool bTriggeredStagger = ApplyPoiseDamage(PoiseDamage);
		if (bTriggeredStagger)
		{
			Kind = EIroncladHitReactionKind::Stagger;
			bIsStagger = true;
		}
	}

	UAnimMontage* MontageToPlay = SelectReactionMontage(Kind);
	if (!MontageToPlay)
	{
		return; // No montage assigned; silent fail is fine for now
	}

	USkeletalMeshComponent* MeshComp = GetMesh();
	UAnimInstance* AnimInst = MeshComp->GetAnimInstance();
	if (!MontageToPlay || !MeshComp || !MeshComp->GetAnimInstance() || !AnimInst)
	{
		UE_LOG(LogIroncladDamage, Error, TEXT("[Damage] TryPlayHitReaction: Abort (missing montage/mesh/animinstance)"));
		return;
	}

	// --- NEW: if stagger, block actions and interrupt current montages BEFORE playing reaction ---
	if (bIsStagger)
	{
		SetActionLockForReaction(true);

		// Interrupt ongoing attack/action montages so stagger truly “wins”
		// This is blunt but effective. Later you can stop only “action” montages if you track them.
		AnimInst->StopAllMontages(0.1f);
	}

	const float PlayResult = AnimInst->Montage_Play(MontageToPlay, 1.0f);

	if (PlayResult > 0.f)
	{
		SetReactingLocked();

		BindReactionMontageEnd(AnimInst, MontageToPlay, bIsStagger);
	}
	else
	{
		// If we locked but failed to play, don't leave the gate locked.
		if (bIsStagger)
		{
			SetActionLockForReaction(false);
		}
	}
}

void AIroncladCharacterBase::BindReactionMontageEnd(UAnimInstance* AnimInst, UAnimMontage* Montage, bool bWasStagger)
{
	if (!AnimInst || !Montage) return;

	bLastReactionWasStagger = bWasStagger;

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AIroncladCharacterBase::OnReactionMontageEnded);

	AnimInst->Montage_SetEndDelegate(EndDelegate, Montage);
}

void AIroncladCharacterBase::OnReactionMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// Your existing reacting unlock probably lives elsewhere; don't break it.
	// Only unlock the combat-gate reaction lock if this reaction was stagger.
	if (bLastReactionWasStagger && bReactionActionLocked)
	{
		SetActionLockForReaction(false);
	}

	bLastReactionWasStagger = false;
}

void AIroncladCharacterBase::SetActionLockForReaction(bool bLocked)
{
	if (UIroncladCombatGateComponent* Gate = GetCombatGate())
	{
		Gate->SetReactionLocked(bLocked);
		bReactionActionLocked = bLocked;
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
