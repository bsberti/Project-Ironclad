#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IroncladCharacterBase.generated.h"

UCLASS()
class IRONCLAD_API AIroncladCharacterBase : public ACharacter
{
    GENERATED_BODY()

public:
    AIroncladCharacterBase();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
};
