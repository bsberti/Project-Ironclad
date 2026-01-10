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

    /** Camera boom positioning the camera behind the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class USpringArmComponent* CameraBoom;

    /** Follow camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class UCameraComponent* FollowCamera;

public:
    virtual void Tick(float DeltaTime) override;
};
