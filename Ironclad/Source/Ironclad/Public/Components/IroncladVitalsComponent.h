#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IroncladVitalsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FIroncladVitalsChanged, float, Current, float, Max);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FIroncladDeathEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IRONCLAD_API UIroncladVitalsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UIroncladVitalsComponent();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vitals|Health")
    float MaxHealth = 100.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vitals|Health")
    float Health = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vitals|Stamina")
    float MaxStamina = 100.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vitals|Stamina")
    float Stamina = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vitals|Stamina")
    float StaminaRegenRate = 25.f; // per second

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vitals|Stamina")
    float StaminaRegenDelay = 0.75f; // seconds after spending

    UPROPERTY(BlueprintAssignable, Category = "Vitals|Events")
    FIroncladVitalsChanged OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "Vitals|Events")
    FIroncladVitalsChanged OnStaminaChanged;

    UPROPERTY(BlueprintAssignable, Category = "Vitals|Events")
    FIroncladDeathEvent OnDeath;

    UFUNCTION(BlueprintCallable, Category = "Vitals|Health")
    bool ApplyDamage(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Vitals|Health")
    bool Heal(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Vitals|Stamina")
    bool SpendStamina(float Amount);

    UFUNCTION(BlueprintPure, Category = "Vitals")
    bool IsDead() const { return bIsDead; }

    UFUNCTION(BlueprintPure, Category = "Vitals")
    float GetHealthNormalized() const;

    UFUNCTION(BlueprintPure, Category = "Vitals")
    float GetStaminaNormalized() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
    bool bIsDead = false;
    bool bStaminaRegenEnabled = true;
    float TimeSinceStaminaSpend = 0.f;

    void BroadcastHealth();
    void BroadcastStamina();
    void HandleDeath();
		
};
