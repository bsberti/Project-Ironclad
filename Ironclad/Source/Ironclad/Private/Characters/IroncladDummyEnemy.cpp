#include "AI/IroncladEnemyAIController.h"

#include "Characters/IroncladDummyEnemy.h"

#include "Components/IroncladVitalsComponent.h"
#include "Components/CapsuleComponent.h"

#include "Combat/Damage/IroncladDamageReceiverComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

AIroncladDummyEnemy::AIroncladDummyEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	AIControllerClass = AIroncladEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Configure the base-created receiver (do NOT create a new one here).
	if (DamageReceiver)
	{
		DamageReceiver->bLogDamage = true;
	}

	// Dummy should not move (test target)
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->DisableMovement();
	}
}

void AIroncladDummyEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (VitalsComponent)
	{
		VitalsComponent->InitializeVitals(200.f); // Dummy has a lot of health
	}
}

void AIroncladDummyEnemy::HandleDeath()
{
	// Always let base perform the shared "death baseline"
	Super::HandleDeath();

	// Dummy-specific policy (optional):
	// - If you want the body to remain targetable by traces but not block movement, you can tune collision here.
	// - Otherwise, keep it minimal for now.
	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Later (ragdoll card): enable mesh physics, adjust mesh collision, etc.
	// SetLifeSpan(10.f); // optional cleanup
}
