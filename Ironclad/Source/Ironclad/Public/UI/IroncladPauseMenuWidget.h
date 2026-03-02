#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IroncladPauseMenuWidget.generated.h"

class UIroncladSaveSubsystem;
class AIroncladPlayerController;

/**
 * Pause Menu base widget.
 * Exposes clean Blueprint-callable handlers so the Widget Blueprint stays simple.
 */
UCLASS()
class IRONCLAD_API UIroncladPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Called by the widget when the Resume button is pressed.
	UFUNCTION(BlueprintCallable, Category = "Ironclad|PauseMenu")
	void OnResumePressed();

	// Called by the widget when the Restart button is pressed.
	UFUNCTION(BlueprintCallable, Category = "Ironclad|PauseMenu")
	void OnRestartPressed();

	// Called by the widget when the Save button is pressed.
	UFUNCTION(BlueprintCallable, Category = "Ironclad|PauseMenu")
	void OnSavePressed();

	// Called by the widget when the Load button is pressed.
	UFUNCTION(BlueprintCallable, Category = "Ironclad|PauseMenu")
	void OnLoadPressed();

	// For enabling/disabling the Load button in Blueprint.
	UFUNCTION(BlueprintPure, Category = "Ironclad|PauseMenu")
	bool CanLoad() const;

protected:
	UIroncladSaveSubsystem* GetSaveSubsystem() const;
	AIroncladPlayerController* GetIroncladPC() const;
};