#include "UI/IroncladPauseMenuWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

#include "Save/IroncladSaveSubsystem.h"
#include "Characters/IroncladPlayerController.h"

UIroncladSaveSubsystem* UIroncladPauseMenuWidget::GetSaveSubsystem() const
{
	if (!GetGameInstance()) return nullptr;
	return GetGameInstance()->GetSubsystem<UIroncladSaveSubsystem>();
}

AIroncladPlayerController* UIroncladPauseMenuWidget::GetIroncladPC() const
{
	return Cast<AIroncladPlayerController>(GetOwningPlayer());
}

void UIroncladPauseMenuWidget::OnResumePressed()
{
	if (AIroncladPlayerController* PC = GetIroncladPC())
	{
		PC->ClosePauseMenu();
	}
}

void UIroncladPauseMenuWidget::OnRestartPressed()
{
	// Close menu before restarting to avoid weird input/cursor states lingering.
	if (AIroncladPlayerController* PC = GetIroncladPC())
	{
		PC->ClosePauseMenu();
	}

	UWorld* World = GetWorld();
	if (!World) return;

	const FName CurrentLevelName = FName(*World->GetName());
	UGameplayStatics::OpenLevel(this, CurrentLevelName);
}

void UIroncladPauseMenuWidget::OnSavePressed()
{
	if (UIroncladSaveSubsystem* Save = GetSaveSubsystem())
	{
		Save->SaveNow();
	}
}

void UIroncladPauseMenuWidget::OnLoadPressed()
{
	if (UIroncladSaveSubsystem* Save = GetSaveSubsystem())
	{
		// Optional: close menu first so UI doesn’t remain on top after load.
		if (AIroncladPlayerController* PC = GetIroncladPC())
		{
			PC->ClosePauseMenu();
		}

		Save->LoadNow();
	}
}

bool UIroncladPauseMenuWidget::CanLoad() const
{
	if (const UIroncladSaveSubsystem* Save = GetSaveSubsystem())
	{
		return Save->HasSave();
	}
	return false;
}