#include "Characters/IroncladAggressorEnemy.h"

#include "AIController.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Combat/Damage/IroncladDamageTypes.h" // wherever FIroncladDamageSpec lives in your project
#include "Kismet/KismetSystemLibrary.h"

AIroncladAggressorEnemy::AIroncladAggressorEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponComponent = CreateDefaultSubobject<UIroncladWeaponComponent>(TEXT("WeaponComponent"));
}

void AIroncladAggressorEnemy::BeginPlay()
{
	Super::BeginPlay();
}

bool AIroncladAggressorEnemy::TryStartAttack(AActor* TargetActor)
{
	//UE_LOG(LogTemp, Log, TEXT("[Aggressor] %s attempting attack on %s"), *GetName(), TargetActor ? *TargetActor->GetName() : TEXT("null"));

	if (!TargetActor || !IsValid(TargetActor))
	{
		return false;
	}

	if (bIsAttacking)
	{
		return false;
	}

	if (!AttackMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Aggressor] AttackMontage not set on %s"), *GetName());
		return false;
	}

	const double Now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0;
	if (LastAttackTimeSeconds > 0.0 && (Now - LastAttackTimeSeconds) < AttackCooldown)
	{
		return false;
	}

	const float Dist = FVector::Dist(GetActorLocation(), TargetActor->GetActorLocation());
	if (Dist > AttackRange)
	{
		return false;
	}

	// Start attack
	CurrentTarget = TargetActor;
	bIsAttacking = true;
	bDidHitThisAttack = false;
	LastAttackTimeSeconds = Now;

	// Stop and face target (classic melee feel)
	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		AIC->StopMovement();
		AIC->SetFocus(TargetActor);
	}
	FaceTarget(TargetActor);

	// Play montage
	if (UAnimInstance* Anim = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr)
	{
		const float PlayResult = Anim->Montage_Play(AttackMontage, 1.0f);
		if (PlayResult <= 0.f)
		{
			UE_LOG(LogTemp, Warning, TEXT("[Aggressor] Montage_Play failed on %s"), *GetName());
			EndAttack();
			return false;
		}

		// Ensure we clear bIsAttacking when montage ends
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
			{
				// Only end if it's OUR montage (safety)
				if (Montage == AttackMontage)
				{
					EndAttack();
				}
			});
		Anim->Montage_SetEndDelegate(EndDelegate, AttackMontage);
	}

	return true;
}

void AIroncladAggressorEnemy::ApplyMeleeHitOnce()
{
	if (!bIsAttacking || bDidHitThisAttack)
	{
		return;
	}

	AActor* TargetActor = CurrentTarget.Get();
	if (!TargetActor || !IsValid(TargetActor))
	{
		return;
	}

	// Quick v1: sphere sweep in front of enemy
	const FVector Start = GetActorLocation() + FVector(0, 0, 50.f);
	const FVector Forward = GetActorForwardVector();
	const FVector End = Start + (Forward * HitSweepDistance);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	FHitResult Hit;
	const bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		this,
		Start,
		End,
		HitSweepRadius,
		UEngineTypes::ConvertToTraceType(ECC_Pawn),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		Hit,
		true
	);

	if (!bHit || !Hit.GetActor())
	{
		return;
	}

	// Only damage the intended target (avoid hitting random pawns)
	if (Hit.GetActor() != TargetActor)
	{
		return;
	}

	// Build damage spec (adjust fields to match your struct)
	FIroncladDamageSpec Spec;
	Spec.BaseAmount = DamageAmount;
	Spec.InstigatorActor = this;
	Spec.SourceActor = this;
	Spec.HitLocation = Hit.ImpactPoint;

	// IMPORTANT: Call ApplyDamage on the target (shared pipeline in CharacterBase)
	// If your damage entry point is interface-based, swap this to that.
	if (AIroncladCharacterBase* Victim = Cast<AIroncladCharacterBase>(TargetActor))
	{
		Victim->ApplyDamage(Spec);
		bDidHitThisAttack = true;
	}
}

void AIroncladAggressorEnemy::EndAttack()
{
	bIsAttacking = false;
	bDidHitThisAttack = false;

	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		AIC->ClearFocus(EAIFocusPriority::Gameplay);
	}

	CurrentTarget = nullptr;
}

void AIroncladAggressorEnemy::FaceTarget(AActor* TargetActor)
{
	if (!TargetActor) return;

	const FVector ToTarget = (TargetActor->GetActorLocation() - GetActorLocation());
	const FVector Flat = FVector(ToTarget.X, ToTarget.Y, 0.f);
	if (!Flat.IsNearlyZero())
	{
		const FRotator Desired = Flat.Rotation();
		SetActorRotation(Desired);
	}
}
