#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combat/Damage/IroncladDamageable.h"
#include "IroncladDamageReceiverComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogIroncladDamage, Log, All);

UCLASS(ClassGroup = (Ironclad), meta = (BlueprintSpawnableComponent))
class IRONCLAD_API UIroncladDamageReceiverComponent : public UActorComponent, public IIroncladDamageable
{
	GENERATED_BODY()

public:
	UIroncladDamageReceiverComponent();

	// If you already have Health as a component, wire it here.
	// For now, we keep this component self-contained and log + placeholder.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	bool bLogDamage = true;

	// Example: simple “health” storage (replace with your real health component if you have it)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (ClampMin = "0.0"))
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damage")
	float CurrentHealth = 100.0f;

	virtual void BeginPlay() override;

	// IIroncladDamageable
	virtual FIroncladDamageResult ApplyDamage_Implementation(const FIroncladDamageSpec& Spec) override;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	bool IsDead() const { return CurrentHealth <= 0.0f; }
};
