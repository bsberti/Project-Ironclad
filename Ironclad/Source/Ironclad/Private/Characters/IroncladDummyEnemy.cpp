#include "Characters/IroncladDummyEnemy.h"

#include "Combat/Damage/IroncladDamageReceiverComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AIroncladDummyEnemy::AIroncladDummyEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	DamageReceiver = CreateDefaultSubobject<UIroncladDamageReceiverComponent>(TEXT("DamageReceiver"));
	// Keep receiver logging on for now
	DamageReceiver->bLogDamage = true;

	// Dummy should not move (optional)
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->DisableMovement();
	}
}

FIroncladDamageResult AIroncladDummyEnemy::ApplyDamage_Implementation(const FIroncladDamageSpec& Spec)
{
	UE_LOG(LogIroncladDamage, Log,
		TEXT("[Damage] AIroncladDummyEnemy::ApplyDamage_Implementation called on %s"),
		*GetNameSafe(this));

	if (!DamageReceiver)
	{
		FIroncladDamageResult Result;
		Result.bApplied = false;
		Result.Reason = FName(TEXT("NoDamageReceiver"));
		return Result;
	}

	FIroncladDamageResult Result = DamageReceiver->ApplyDamage_Implementation(Spec);

	if (Result.bApplied && DamageReceiver->IsDead())
	{
		HandleDeath();
	}

	return Result;
}

void AIroncladDummyEnemy::HandleDeath()
{
	// Minimal “death” for test validation: stop collision and movement.
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->DisableMovement();
	}

	// Optional: ragdoll later; for now just hide or stop responding.
	// GetMesh()->SetSimulatePhysics(true);

	// You can also Destroy() after a delay for cleanup, but keeping it around is useful for debugging.
}
