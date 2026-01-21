#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Combat/Damage/IroncladDamageable.h"
#include "IroncladDummyEnemy.generated.h"

class UIroncladDamageReceiverComponent;

UCLASS()
class IRONCLAD_API AIroncladDummyEnemy : public ACharacter, public IIroncladDamageable
{
	GENERATED_BODY()

public:
	AIroncladDummyEnemy();

	// IIroncladDamageable
	virtual FIroncladDamageResult ApplyDamage_Implementation(const FIroncladDamageSpec& Spec) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damage")
	TObjectPtr<UIroncladDamageReceiverComponent> DamageReceiver = nullptr;

	// Minimal death behavior for testing.
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void HandleDeath();
};
