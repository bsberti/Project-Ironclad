
#include "IroncladGameMode.h"

#include "Arena/IroncladEnemySpawnPoint.h"

#include "Components/CapsuleComponent.h"

#include "Characters/IroncladEnemyBase.h"

#include "Kismet/GameplayStatics.h"

#include "GameFramework/Character.h"

AIroncladGameMode::AIroncladGameMode()
{
	// stub
}

static FVector FindGroundSnapLocation(UWorld* World, const FVector& Start, float TraceDown, float CapsuleHalfHeight)
{
	FHitResult Hit;
	const FVector End = Start - FVector(0, 0, TraceDown);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(EnemySpawnTrace), false);
	const bool bHit = World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	const FVector Ground = bHit ? Hit.ImpactPoint : Start;
	return Ground + FVector(0, 0, CapsuleHalfHeight);
}

void AIroncladGameMode::RequestRespawn(AController* Controller, float DelaySeconds)
{
    if (!Controller) return;

    FTimerHandle Handle;
    GetWorldTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([this, Controller]()
        {
            RestartPlayer(Controller);
        }), DelaySeconds, false);
}

void AIroncladGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AIroncladEnemySpawnPoint::StaticClass(), SpawnPoints);

	for (AActor* A : SpawnPoints)
	{
		AIroncladEnemySpawnPoint* SP = Cast<AIroncladEnemySpawnPoint>(A);
		if (!SP || !SP->bSpawnOnBeginPlay)
		{
			continue;
		}

		if (SP->SpawnId.IsNone())
		{
			UE_LOG(LogTemp, Warning, TEXT("[Spawn] SpawnPoint %s has no SpawnId."), *GetNameSafe(SP));
			continue;
		}

		if (!SP->EnemyClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("[Spawn] SpawnPoint %s has no EnemyClass."), *GetNameSafe(SP));
			continue;
		}

		FTransform T = SP->GetSpawnTransform();

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AIroncladEnemyBase* Enemy = GetWorld()->SpawnActorDeferred<AIroncladEnemyBase>(SP->EnemyClass, T);
		if (Enemy)
		{
			const float HalfHeight = Enemy->GetCapsuleComponent()
				? Enemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()
				: 88.f;

			const FVector Start = T.GetLocation() + FVector(0, 0, 50.f); // small up bias
			const FVector Snapped = FindGroundSnapLocation(GetWorld(), Start, 5000.f, HalfHeight);

			T.SetLocation(Snapped);

			Enemy->FinishSpawning(T, true);
		} 
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("[Spawn] Failed to spawn enemy at %s"), *SP->SpawnId.ToString());
			continue;
		}

		// Store SpawnId on the enemy (add this property or setter as described)
		Enemy->SpawnId = SP->SpawnId;
	}
}