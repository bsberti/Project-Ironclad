#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IroncladCharacterBase.generated.h"

class UIroncladVitalsComponent;

UCLASS()
class IRONCLAD_API AIroncladCharacterBase : public ACharacter
{
    GENERATED_BODY()

public:
    AIroncladCharacterBase();

    UFUNCTION(BlueprintCallable, Category = "Vitals")
    bool ApplyDamageToVitals(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Vitals")
    bool HealVitals(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Vitals")
    bool SpendStamina(float Amount);

    UFUNCTION(BlueprintPure, Category = "Vitals")
    bool IsDead() const;

    UFUNCTION(BlueprintPure, Category = "Vitals")
    UIroncladVitalsComponent* GetVitals() const { return VitalsComponent; }

    // Route engine damage into our system
    virtual float TakeDamage(
        float DamageAmount,
        struct FDamageEvent const& DamageEvent,
        class AController* EventInstigator,
        AActor* DamageCauser
    ) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UIroncladVitalsComponent* VitalsComponent;

    UFUNCTION()
    void HandleDeath();

public:
    virtual void Tick(float DeltaTime) override;
};
