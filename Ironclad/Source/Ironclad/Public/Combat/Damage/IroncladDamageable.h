#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Combat/Damage/IroncladDamageTypes.h"
#include "IroncladDamageable.generated.h"

UINTERFACE(BlueprintType)
class IRONCLAD_API UIroncladDamageable : public UInterface
{
	GENERATED_BODY()
};

class IRONCLAD_API IIroncladDamageable
{
	GENERATED_BODY()

public:
	// BlueprintNativeEvent lets both C++ and BP targets implement it.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	FIroncladDamageResult ApplyDamage(const FIroncladDamageSpec& Spec);
};
