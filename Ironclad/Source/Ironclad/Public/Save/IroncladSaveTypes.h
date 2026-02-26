#pragma once

#include "CoreMinimal.h"
#include "IroncladSaveTypes.generated.h"

USTRUCT(BlueprintType)
struct FIroncladCooldownSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AbilityId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Remaining = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 0.f;
};

USTRUCT(BlueprintType)
struct FIroncladPlayerSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Stamina = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxStamina = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FIroncladCooldownSaveData> Cooldowns;
};

USTRUCT(BlueprintType)
struct FIroncladArenaSaveData
{
	GENERATED_BODY()

	// Keep it simple first. Expand later.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CheckpointId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ArenaProgressIndex = 0;
};