#include "Animation/ANS_IroncladHitWindow.h"

#include "Components/IroncladWeaponComponent.h"
#include "Weapons/IroncladWeaponActor.h"

#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"

void UANS_IroncladHitWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (!MeshComp)
	{
		return;
	}

	AActor* Owner = MeshComp->GetOwner();
	AIroncladWeaponActor* Weapon = GetEquippedWeapon(Owner);
	if (!Weapon)
	{
		return;
	}

	UIroncladHitDetectionComponent* HitDetect = Weapon->GetHitDetectionComponent();
	USceneComponent* TraceSource = Weapon->GetTraceSourceComponent();

	if (!HitDetect || !TraceSource)
	{
		return;
	}

	HitDetect->BeginHitWindow(TraceSource, TraceConfig);
}

void UANS_IroncladHitWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (!MeshComp)
	{
		return;
	}

	AActor* Owner = MeshComp->GetOwner();
	AIroncladWeaponActor* Weapon = GetEquippedWeapon(Owner);
	if (!Weapon)
	{
		return;
	}

	if (UIroncladHitDetectionComponent* HitDetect = Weapon->GetHitDetectionComponent())
	{
		HitDetect->EndHitWindow();
	}
}

UIroncladWeaponComponent* UANS_IroncladHitWindow::FindWeaponComponent(AActor* Owner) const
{
	if (!Owner)
	{
		return nullptr;
	}

	return Owner->FindComponentByClass<UIroncladWeaponComponent>();
}

AIroncladWeaponActor* UANS_IroncladHitWindow::GetEquippedWeapon(AActor* Owner) const
{
	UIroncladWeaponComponent* WeaponComp = FindWeaponComponent(Owner);
	if (!WeaponComp)
	{
		return nullptr;
	}

	return WeaponComp->GetEquippedWeaponActor();
}
