#include "Animation/ANS_IroncladComboWindow.h"

#include "GameFramework/Actor.h"
#include "Characters/IroncladPlayerCharacter.h"
#include "Components/IroncladComboComponent.h"

void UANS_IroncladComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration);

    if (!MeshComp) return;

    AActor* Owner = MeshComp->GetOwner();
    AIroncladPlayerCharacter* Character = Cast<AIroncladPlayerCharacter>(Owner);
    if (!Character) return;

    if (UIroncladComboComponent* Combo = Character->FindComponentByClass<UIroncladComboComponent>())
    {
        Combo->OpenChainWindow(DebugTag);
    }
}

void UANS_IroncladComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::NotifyEnd(MeshComp, Animation);

    if (!MeshComp) return;

    AActor* Owner = MeshComp->GetOwner();
    AIroncladPlayerCharacter* Character = Cast<AIroncladPlayerCharacter>(Owner);
    if (!Character) return;

    if (UIroncladComboComponent* Combo = Character->FindComponentByClass<UIroncladComboComponent>())
    {
        Combo->CloseChainWindow(DebugTag);
    }
}
