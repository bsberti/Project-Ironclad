#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_IroncladComboWindow.generated.h"

UCLASS()
class IRONCLAD_API UANS_IroncladComboWindow : public UAnimNotifyState
{
    GENERATED_BODY()

public:
    // Optional tag to help debugging different windows
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
    FName DebugTag = NAME_None;

    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
