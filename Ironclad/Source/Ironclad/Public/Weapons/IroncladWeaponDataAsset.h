#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IroncladWeaponDataAsset.generated.h"

class UAnimMontage;
class AIroncladWeaponActor;

UENUM(BlueprintType)
enum class EWeaponTraceShape : uint8
{
    Line,
    Sphere
};

UCLASS(BlueprintType)
class IRONCLAD_API UIroncladWeaponDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    // Identity / presentation
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Identity")
    FName WeaponId = NAME_None;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Identity")
    FText DisplayName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Equip")
    FName AttachSocketName = TEXT("hand_r_socket"); // adjust to your skeleton

    // Visual / actor to spawn
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Equip")
    TSubclassOf<AIroncladWeaponActor> WeaponActorClass;

    // Combat tuning
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Attacks")
    TObjectPtr<UAnimMontage> LightAttackMontage = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Attacks")
    TObjectPtr<UAnimMontage> HeavyAttackMontage = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Costs", meta = (ClampMin = "0"))
    float LightAttackStaminaCost = 10.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Costs", meta = (ClampMin = "0"))
    float HeavyAttackStaminaCost = 20.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Damage", meta = (ClampMin = "0"))
    float LightAttackDamage = 10.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Damage", meta = (ClampMin = "0"))
    float HeavyAttackDamage = 20.f;

    // Hit detection tuning (foundation only; actual traces later)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Trace")
    FName TraceProfileName = TEXT("WeaponTrace");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Trace")
    EWeaponTraceShape TraceShape = EWeaponTraceShape::Sphere;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Trace", meta = (ClampMin = "0"))
    float TraceRange = 150.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Trace", meta = (ClampMin = "0"))
    float TraceRadius = 15.f;
};
