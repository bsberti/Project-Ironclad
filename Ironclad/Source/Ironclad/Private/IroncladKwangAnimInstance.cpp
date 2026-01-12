#include "IroncladKwangAnimInstance.h"
#include "GameFramework/Pawn.h"
#include "../Public/Characters/IroncladPlayerCharacter.h"

void UIroncladKwangAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    APawn* PawnOwner = TryGetPawnOwner();
    if (!PawnOwner)
    {
        bIsSprinting = false;
        return;
    }

    const AIroncladPlayerCharacter* Player = Cast<AIroncladPlayerCharacter>(PawnOwner);
    bIsSprinting = Player ? Player->IsSprinting() : false; // if private, we’ll add a getter
}
