#include "Components/IroncladWeaponComponent.h"
#include "Weapons/IroncladWeaponDataAsset.h"
#include "Weapons/IroncladWeaponActor.h"

#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"

UIroncladWeaponComponent::UIroncladWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UIroncladWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    if (WeaponLoadout.Num() > 0 && WeaponLoadout[0])
    {
        EquipWeapon(WeaponLoadout[0]);
    }
    else if (DefaultWeaponData)
    {
        EquipWeapon(DefaultWeaponData);
    }

}

bool UIroncladWeaponComponent::EquipWeapon(UIroncladWeaponDataAsset* NewWeaponData)
{
    if (!NewWeaponData)
    {
        return false;
    }

    // Replace existing weapon
    UnequipWeapon();

    if (!SpawnAndAttachWeaponActor(NewWeaponData))
    {
        return false;
    }

    CurrentWeaponData = NewWeaponData;
    return true;
}

void UIroncladWeaponComponent::UnequipWeapon()
{
    CurrentWeaponData = nullptr;

    if (EquippedWeaponActor)
    {
        EquippedWeaponActor->Destroy();
        EquippedWeaponActor = nullptr;
    }
}

USkeletalMeshComponent* UIroncladWeaponComponent::FindOwnerSkeletalMesh() const
{
    if (!GetOwner())
    {
        return nullptr;
    }

    // Common case: character owns a SkeletalMeshComponent
    return GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
}

bool UIroncladWeaponComponent::SpawnAndAttachWeaponActor(UIroncladWeaponDataAsset* WeaponData)
{
    if (!GetOwner() || !GetWorld() || !WeaponData || !WeaponData->WeaponActorClass)
    {
        return false;
    }

    USkeletalMeshComponent* OwnerMesh = FindOwnerSkeletalMesh();
    if (!OwnerMesh)
    {
        return false;
    }

    FActorSpawnParameters Params;
    Params.Owner = GetOwner();
    Params.Instigator = GetOwner()->GetInstigator();
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AIroncladWeaponActor* Spawned = GetWorld()->SpawnActor<AIroncladWeaponActor>(
        WeaponData->WeaponActorClass,
        FTransform::Identity,
        Params
    );

    if (!Spawned)
    {
        return false;
    }

    Spawned->InitializeFromData(WeaponData);

    Spawned->AttachToComponent(
        OwnerMesh,
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        WeaponData->AttachSocketName
    );

    EquippedWeaponActor = Spawned;
    return true;
}

bool UIroncladWeaponComponent::CycleWeapon()
{
    if (WeaponLoadout.Num() < 2)
    {
        return false;
    }

    // Find current index
    int32 CurrentIndex = INDEX_NONE;
    for (int32 i = 0; i < WeaponLoadout.Num(); ++i)
    {
        if (WeaponLoadout[i] == CurrentWeaponData)
        {
            CurrentIndex = i;
            break;
        }
    }

    // If current not found, start at 0
    const int32 NextIndex = (CurrentIndex == INDEX_NONE)
        ? 0
        : (CurrentIndex + 1) % WeaponLoadout.Num();

    return EquipWeapon(WeaponLoadout[NextIndex]);
}
