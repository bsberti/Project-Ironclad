#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IroncladHUDWidget.generated.h"

UCLASS(Abstract)
class IRONCLAD_API UIroncladHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// These are UI-facing hooks. Implement in your WBP (Blueprint child).
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void SetHealth(float Current, float Max);

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void SetStamina(float Current, float Max);

	// Step 4.1 later: cooldowns (we’ll wire these after the HUD is up)
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void SetAbilityCooldown(FName AbilityId, float Remaining, float Duration);

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void ClearAbilityCooldown(FName AbilityId);
};
