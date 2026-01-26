#include "Characters/IroncladEnemyBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogIroncladEnemyBase, Log, All);

AIroncladEnemyBase::AIroncladEnemyBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AIroncladEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogIroncladEnemyBase, Verbose, TEXT("[EnemyBase] %s BeginPlay. Faction=%d"), *GetName(), (int32)Faction);
}

void AIroncladEnemyBase::HandleDeath()
{
	// Keep shared baseline behavior centralized in CharacterBase
	Super::HandleDeath();

	// Enemy-level hook point (keep minimal for now)
	UE_LOG(LogIroncladEnemyBase, Display, TEXT("[EnemyBase] %s died."), *GetName());
}

void AIroncladEnemyBase::OnHitReceived_Implementation()
{
	// Placeholder for Card 3.7
}