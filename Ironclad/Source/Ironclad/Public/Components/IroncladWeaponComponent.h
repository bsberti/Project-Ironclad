#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IroncladWeaponComponent.generated.h"

class UIroncladWeaponDataAsset;
class AIroncladWeaponActor;
class USkeletalMeshComponent;

UCLASS(ClassGroup = (Ironclad), meta = (BlueprintSpawnableComponent))
class IRONCLAD_API UIroncladWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UIroncladWeaponComponent();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    bool EquipWeapon(UIroncladWeaponDataAsset* NewWeaponData);

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void UnequipWeapon();

    UFUNCTION(BlueprintPure, Category = "Weapon")
    UIroncladWeaponDataAsset* GetCurrentWeaponData() const { return CurrentWeaponData; }

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    AIroncladWeaponActor* GetEquippedWeaponActor() const { return EquippedWeaponActor; }

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    bool CycleWeapon();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Defaults")
    TObjectPtr<UIroncladWeaponDataAsset> DefaultWeaponData = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Defaults")
    TArray<TObjectPtr<UIroncladWeaponDataAsset>> WeaponLoadout;

private:
    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    TObjectPtr<UIroncladWeaponDataAsset> CurrentWeaponData = nullptr;

    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    TObjectPtr<AIroncladWeaponActor> EquippedWeaponActor = nullptr;

    USkeletalMeshComponent* FindOwnerSkeletalMesh() const;

    bool SpawnAndAttachWeaponActor(UIroncladWeaponDataAsset* WeaponData);
};
