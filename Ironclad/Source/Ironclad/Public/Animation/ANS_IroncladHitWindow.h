#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Components/IroncladHitDetectionComponent.h" // for FIroncladTraceConfig
#include "ANS_IroncladHitWindow.generated.h"

UCLASS(meta = (DisplayName = "Ironclad Hit Window"))
class IRONCLAD_API UANS_IroncladHitWindow : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	// Per-notify config (lets you tune per montage section/step).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitDetection")
	FIroncladTraceConfig TraceConfig;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	// Helpers (kept private to avoid leaking weapon/equipment assumptions elsewhere).
	class UIroncladWeaponComponent* FindWeaponComponent(AActor* Owner) const;
	class AIroncladWeaponActor* GetEquippedWeapon(AActor* Owner) const;
};
