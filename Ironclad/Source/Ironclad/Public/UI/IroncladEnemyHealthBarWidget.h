#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IroncladEnemyHealthBarWidget.generated.h"

UCLASS()
class IRONCLAD_API UIroncladEnemyHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "EnemyHealth")
	void SetHealth(float Current, float Max);
};
