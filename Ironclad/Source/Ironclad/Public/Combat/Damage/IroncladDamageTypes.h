#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "IroncladDamageTypes.generated.h"

UENUM(BlueprintType)
enum class EIroncladDamageType : uint8
{
	Physical UMETA(DisplayName = "Physical"),
	Slash    UMETA(DisplayName = "Slash"),
	Pierce   UMETA(DisplayName = "Pierce"),
	Blunt    UMETA(DisplayName = "Blunt"),
	Magic    UMETA(DisplayName = "Magic")
};

USTRUCT(BlueprintType)
struct FIroncladDamageSpec
{
	GENERATED_BODY()

	// --- Source context ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TObjectPtr<AActor> SourceActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TObjectPtr<AController> SourceController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TObjectPtr<AActor> InstigatorActor = nullptr;

	// Optional “what did the damage” object (weapon actor, ability object, etc.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TObjectPtr<UObject> SourceObject = nullptr;

	// --- Damage payload ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (ClampMin = "0.0"))
	float BaseAmount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (ClampMin = "0.0"))
	float FinalAmount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	EIroncladDamageType DamageType = EIroncladDamageType::Physical;

	// Generic tags for downstream logic: "Damage.Slash", "Attack.Light", "Weapon.Sword", etc.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	FGameplayTagContainer Tags;

	// --- Hit information ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	FHitResult Hit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	FVector HitLocation = FVector::ZeroVector;

	// Convenience: call after filling BaseAmount + modifiers.
	void Finalize()
	{
		FinalAmount = FMath::Max(0.0f, FinalAmount > 0.0f ? FinalAmount : BaseAmount);
	}
};

USTRUCT(BlueprintType)
struct FIroncladDamageResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	bool bApplied = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float AppliedAmount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	FName Reason = NAME_None; // e.g. "Dead", "Invulnerable", "Ignored"
};
