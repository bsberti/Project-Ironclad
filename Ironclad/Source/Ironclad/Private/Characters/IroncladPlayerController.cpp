// Copyright Epic Games, Inc. All Rights Reserved.


#include "Characters/IroncladPlayerController.h"

#include "UI/IroncladHUDSubsystem.h"

#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "Ironclad.h"
#include "Widgets/Input/SVirtualJoystick.h"

void AIroncladPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UGameInstance* GI = GetGameInstance())
	{
		if (UIroncladHUDSubsystem* HUD = GI->GetSubsystem<UIroncladHUDSubsystem>())
		{
			HUD->EnsureHUD();
		}
	}

	// only spawn touch controls on local player controllers
	if (ShouldUseTouchControls() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogIronclad, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void AIroncladPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}

bool AIroncladPlayerController::ShouldUseTouchControls() const
{
	// are we on a mobile platform? Should we force touch?
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}

bool AIroncladPlayerController::IsPauseMenuOpen() const
{
	return ActivePauseMenu && ActivePauseMenu->IsInViewport();
}

void AIroncladPlayerController::ApplyPauseInputMode(bool bEnableUI)
{
	if (bEnableUI)
	{
		bShowMouseCursor = true;
		FInputModeGameAndUI Mode;
		Mode.SetHideCursorDuringCapture(false);
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		Mode.SetWidgetToFocus(ActivePauseMenu
			? TSharedPtr<SWidget>(ActivePauseMenu->TakeWidget())
			: TSharedPtr<SWidget>(nullptr));
		SetInputMode(Mode);
	}
	else
	{
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
}

void AIroncladPlayerController::TogglePauseMenu()
{
	if (IsPauseMenuOpen()) ClosePauseMenu();
	else OpenPauseMenu();
}

void AIroncladPlayerController::OpenPauseMenu()
{
	if (IsPauseMenuOpen() || !PauseMenuClass) return;

	SetPause(true);

	ActivePauseMenu = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (!ActivePauseMenu) return;

	ActivePauseMenu->AddToViewport(100);
	ApplyPauseInputMode(true);
}

void AIroncladPlayerController::ClosePauseMenu()
{
	if (!IsPauseMenuOpen()) return;

	if (ActivePauseMenu)
	{
		ActivePauseMenu->RemoveFromParent();
		ActivePauseMenu = nullptr;
	}

	SetPause(false);
	ApplyPauseInputMode(false);
}