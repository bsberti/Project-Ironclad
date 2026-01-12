#include "IroncladCharacterBase.h"

AIroncladCharacterBase::AIroncladCharacterBase()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AIroncladCharacterBase::BeginPlay()
{
    Super::BeginPlay();
}

void AIroncladCharacterBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
