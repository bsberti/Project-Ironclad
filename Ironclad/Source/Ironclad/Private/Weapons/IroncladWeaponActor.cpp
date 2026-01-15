#include "Weapons/IroncladWeaponActor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Weapons/IroncladWeaponDataAsset.h"

AIroncladWeaponActor::AIroncladWeaponActor()
{
    PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    Mesh->SetupAttachment(Root);

    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AIroncladWeaponActor::InitializeFromData(UIroncladWeaponDataAsset* InWeaponData)
{
    WeaponData = InWeaponData;
    // Foundation: no mesh assignment here unless you choose to store a mesh in the DataAsset.
}
