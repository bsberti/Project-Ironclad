#include "Weapons/IroncladWeaponActor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Weapons/IroncladWeaponDataAsset.h"
#include "Components/IroncladHitDetectionComponent.h"

AIroncladWeaponActor::AIroncladWeaponActor()
{
    PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    Mesh->SetupAttachment(Root);

    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    HitDetectionComponent = CreateDefaultSubobject<UIroncladHitDetectionComponent>(TEXT("HitDetection"));
}

USceneComponent* AIroncladWeaponActor::GetTraceSourceComponent() const
{
    // The HitDetectionComponent expects a USceneComponent with sockets.
    // For a StaticMeshComponent, sockets are supported if you created them in the asset.
    // If you use SkeletalMeshComponent, this also works.
    return Cast<USceneComponent>(Mesh);
}

void AIroncladWeaponActor::InitializeFromData(UIroncladWeaponDataAsset* InWeaponData)
{
    WeaponData = InWeaponData;
    // Foundation: no mesh assignment here unless you choose to store a mesh in the DataAsset.
}
