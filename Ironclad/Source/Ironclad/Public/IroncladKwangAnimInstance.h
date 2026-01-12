#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "IroncladKwangAnimInstance.generated.h"

UCLASS()
class IRONCLAD_API UIroncladKwangAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
    // Exposed to AnimBP graphs
    UPROPERTY(BlueprintReadOnly, Category = "Ironclad|Animation")
    bool bIsSprinting = false;

protected:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};