#pragma once

#include "Weapons/IroncladWeaponDataAsset.h"

#include "Components/SceneComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IroncladWeaponActor.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UIroncladWeaponDataAsset;

class UIroncladHitDetectionComponent;
class USceneComponent;

UCLASS()
class IRONCLAD_API AIroncladWeaponActor : public AActor
{
    GENERATED_BODY()

public:
    AIroncladWeaponActor();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void InitializeFromData(UIroncladWeaponDataAsset* InWeaponData);

    UFUNCTION(BlueprintPure, Category = "Weapon")
    UIroncladWeaponDataAsset* GetWeaponData() const { return WeaponData; }

    // Hit detection accessors used by the AnimNotifyState.
    UFUNCTION(BlueprintCallable, Category = "HitDetection")
    UIroncladHitDetectionComponent* GetHitDetectionComponent() const { return HitDetectionComponent; }

    UFUNCTION(BlueprintCallable, Category = "HitDetection")
    USceneComponent* GetTraceSourceComponent() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    TObjectPtr<USceneComponent> Root = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    TObjectPtr<UStaticMeshComponent> Mesh = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    TObjectPtr<UIroncladWeaponDataAsset> WeaponData = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UIroncladHitDetectionComponent> HitDetectionComponent = nullptr;

};
