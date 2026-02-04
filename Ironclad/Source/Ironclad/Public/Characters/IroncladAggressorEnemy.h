#pragma once

#include "CoreMinimal.h"

#include "Components/IroncladWeaponComponent.h"

#include "Characters/IroncladEnemyBase.h"

#include "IroncladAggressorEnemy.generated.h"

class UAnimMontage;
class UIroncladWeaponComponent;

UCLASS()
class IRONCLAD_API AIroncladAggressorEnemy : public AIroncladEnemyBase
{
	GENERATED_BODY()

public:
	AIroncladAggressorEnemy();

	// Called by BT task. Returns true only when an attack was successfully started.
	UFUNCTION(BlueprintCallable, Category = "Ironclad|Enemy|Aggressor")
	bool TryStartAttack(AActor* TargetActor);

	// Called from an AnimNotify (quick v1) to apply damage once during the swing.
	// Later, replace with your notify-window hit detect pipeline.
	UFUNCTION(BlueprintCallable, Category = "Ironclad|Enemy|Aggressor")
	void ApplyMeleeHitOnce();

	// BT/Debug convenience
	UFUNCTION(BlueprintPure, Category = "Ironclad|Enemy|Aggressor")
	bool IsAttacking() const { return bIsAttacking; }

	UFUNCTION(BlueprintPure, Category = "Ironclad|Enemy|Aggressor")
	float GetAttackRange() const { return AttackRange; }

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

protected:
	virtual void BeginPlay() override;

	// Attack tuning
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ironclad|Enemy|Aggressor|Attack")
	UAnimMontage* AttackMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ironclad|Enemy|Aggressor|Attack", meta = (ClampMin = "0.0"))
	float AttackRange = 175.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ironclad|Enemy|Aggressor|Attack", meta = (ClampMin = "0.0"))
	float AttackCooldown = 1.25f;

	// Damage tuning (v1)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ironclad|Enemy|Aggressor|Damage", meta = (ClampMin = "0.0"))
	float DamageAmount = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ironclad|Enemy|Aggressor|Damage", meta = (ClampMin = "0.0"))
	float HitSweepRadius = 25.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ironclad|Enemy|Aggressor|Damage", meta = (ClampMin = "0.0"))
	float HitSweepDistance = 120.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UIroncladWeaponComponent* WeaponComponent;

private:
	UPROPERTY(Transient)
	TObjectPtr<AActor> CurrentTarget = nullptr;

	UPROPERTY(Transient)
	bool bIsAttacking = false;

	double LastAttackTimeSeconds = -1.0;

	// Prevent hitting multiple times per montage (v1).
	bool bDidHitThisAttack = false;

	void EndAttack();

	// Small helper for facing target (Aggressor vibe)
	void FaceTarget(AActor* TargetActor);
};
