#include "Weapons/IroncladWeaponActor.h"
#include "Weapons/IroncladWeaponDataAsset.h"

#include "Combat/Damage/IroncladDamageable.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
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

void AIroncladWeaponActor::BeginPlay()
{
    Super::BeginPlay();

    if (HitDetectionComponent)
    {
        // Dynamic multicast: AddDynamic is correct here.
        HitDetectionComponent->OnHitDetected.AddDynamic(this, &AIroncladWeaponActor::HandleWeaponHit);
    }
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

void AIroncladWeaponActor::HandleWeaponHit(const FHitResult& Hit)
{
	AActor* Target = Hit.GetActor();
	if (!Target)
	{
		return;
	}

	FIroncladDamageSpec Spec = BuildDamageSpecFromHit(Hit);
	Spec.Finalize();

	// Apply via interface if possible (works for both C++ and BP implementations).
	if (Target->GetClass()->ImplementsInterface(UIroncladDamageable::StaticClass()))
	{
		const FIroncladDamageResult Result = IIroncladDamageable::Execute_ApplyDamage(Target, Spec);
		// Optional: log failure reasons here if Result.bApplied == false
	}
}

FIroncladDamageSpec AIroncladWeaponActor::BuildDamageSpecFromHit(const FHitResult& Hit) const
{
	FIroncladDamageSpec Spec;
	Spec.Hit = Hit;

	Spec.SourceActor = GetOwner();           // owner is the character holding the weapon
	Spec.InstigatorActor = GetInstigator();  // typically same as owner but safe
	Spec.SourceController = (Spec.InstigatorActor) ? Spec.InstigatorActor->GetInstigatorController() : nullptr;
	Spec.SourceObject = const_cast<AIroncladWeaponActor*>(this);

	if (WeaponData)
	{
		Spec.BaseAmount = WeaponData->BaseDamage;
		Spec.FinalAmount = WeaponData->BaseDamage; // no modifiers yet
		Spec.DamageType = WeaponData->DamageType;
		Spec.Tags.AppendTags(WeaponData->DamageTags);
	}

	// Optional: add contextual tags (later: Attack.Light/Heavy, Combo.StepN, etc.)
	// Spec.Tags.AddTagFast(...);

	return Spec;
}
