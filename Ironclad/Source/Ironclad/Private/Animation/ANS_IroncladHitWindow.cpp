#include "Animation/ANS_IroncladHitWindow.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"

// Include your character header so we can cast and call SetHitWindowActive
#include "Characters/IroncladPlayerCharacter.h"

void UANS_IroncladHitWindow::NotifyBegin(
    USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation,
    float TotalDuration,
    const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

    if (!MeshComp)
    {
        return;
    }

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner)
    {
        return;
    }

    // If you want this to work for both player + enemies later, consider casting to a base interface.
    if (AIroncladPlayerCharacter* PlayerChar = Cast<AIroncladPlayerCharacter>(Owner))
    {
        PlayerChar->SetHitWindowActive(true);
    }
}

void UANS_IroncladHitWindow::NotifyEnd(
    USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation,
    const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyEnd(MeshComp, Animation, EventReference);

    if (!MeshComp)
    {
        return;
    }

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner)
    {
        return;
    }

    if (AIroncladPlayerCharacter* PlayerChar = Cast<AIroncladPlayerCharacter>(Owner))
    {
        PlayerChar->SetHitWindowActive(false);
    }
}
