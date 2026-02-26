#include "Save/IroncladSaveSubsystem.h"
#include "Save/IroncladSaveGame.h"
#include "Save/IroncladSaveTypes.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

#include "Components/IroncladVitalsComponent.h"
#include "Components/IroncladAbilityComponent.h"

DEFINE_LOG_CATEGORY(LogIroncladSave);

void UIroncladSaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SaveGameClass = UIroncladSaveGame::StaticClass();
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UIroncladSaveSubsystem::HandlePostLoadMap);

	UE_LOG(LogIroncladSave, Display, TEXT("[Save] Initialized. Slot=%s"), *SlotName);
}

void UIroncladSaveSubsystem::Deinitialize()
{
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
	Super::Deinitialize();
}

bool UIroncladSaveSubsystem::HasSave() const
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex);
}

bool UIroncladSaveSubsystem::GetLocalPlayerPawn(APawn*& OutPawn) const
{
	OutPawn = nullptr;

	if (!GetWorld())
	{
		return false;
	}

	UGameInstance* GI = GetGameInstance();
	if (!GI)
	{
		return false;
	}

	APlayerController* PC = GI->GetFirstLocalPlayerController(GetWorld());
	if (!PC)
	{
		return false;
	}

	OutPawn = PC->GetPawn();
	return (OutPawn != nullptr);
}

UIroncladSaveGame* UIroncladSaveSubsystem::BuildSaveObject() const
{
	APawn* Pawn = nullptr;
	if (!GetLocalPlayerPawn(Pawn))
	{
		UE_LOG(LogIroncladSave, Warning, TEXT("[Save] BuildSaveObject failed: No local pawn."));
		return nullptr;
	}

	UIroncladSaveGame* SaveObj = Cast<UIroncladSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	if (!SaveObj)
	{
		UE_LOG(LogIroncladSave, Warning, TEXT("[Save] CreateSaveGameObject failed."));
		return nullptr;
	}

	// Transform
	SaveObj->Player.Location = Pawn->GetActorLocation();
	SaveObj->Player.Rotation = Pawn->GetActorRotation();

	// Vitals
	if (UIroncladVitalsComponent* Vitals = Pawn->FindComponentByClass<UIroncladVitalsComponent>())
	{
		SaveObj->Player.Health = Vitals->GetHealth();
		SaveObj->Player.MaxHealth = Vitals->GetMaxHealth();
		SaveObj->Player.Stamina = Vitals->GetStamina();
		SaveObj->Player.MaxStamina = Vitals->GetMaxStamina();
	}
	else
	{
		UE_LOG(LogIroncladSave, Warning, TEXT("[Save] Pawn has no VitalsComponent: %s"), *GetNameSafe(Pawn));
	}

	// Abilities cooldowns (deterministic: save remaining)
	if (UIroncladAbilityComponent* Abilities = Pawn->FindComponentByClass<UIroncladAbilityComponent>())
	{
		// Add a small getter to your AbilityComponent (see section below).
		TArray<FIroncladCooldownSaveData> Cooldowns;
		Abilities->GetCooldownsForSave(Cooldowns);

		SaveObj->Player.Cooldowns = Cooldowns;
	}

	// Arena progress: keep simple for now (you can wire this later from an ArenaSubsystem or GameState)
	SaveObj->Arena.CheckpointId = NAME_None;
	SaveObj->Arena.ArenaProgressIndex = 0;

	return SaveObj;
}

bool UIroncladSaveSubsystem::SaveNow()
{
	UIroncladSaveGame* SaveObj = BuildSaveObject();
	if (!SaveObj)
	{
		return false;
	}

	const bool bOK = UGameplayStatics::SaveGameToSlot(SaveObj, SlotName, UserIndex);
	UE_LOG(LogIroncladSave, Display, TEXT("[Save] SaveNow %s"), bOK ? TEXT("SUCCESS") : TEXT("FAILED"));
	return bOK;
}

bool UIroncladSaveSubsystem::ApplySaveToCurrentPawn(const UIroncladSaveGame* SaveObj) const
{
	if (!SaveObj)
	{
		return false;
	}

	APawn* Pawn = nullptr;
	if (!GetLocalPlayerPawn(Pawn))
	{
		UE_LOG(LogIroncladSave, Warning, TEXT("[Save] ApplySave failed: No local pawn yet."));
		return false;
	}

	// Move pawn first
	Pawn->SetActorLocationAndRotation(SaveObj->Player.Location, SaveObj->Player.Rotation, false, nullptr, ETeleportType::TeleportPhysics);

	// Vitals
	if (UIroncladVitalsComponent* Vitals = Pawn->FindComponentByClass<UIroncladVitalsComponent>())
	{
		// Use your component’s “authoritative” setters (whatever you already have).
		// If you don't have them yet, add SetHealth/SetStamina as safe clamped setters.
		Vitals->SetHealth(SaveObj->Player.Health);
		Vitals->SetStamina(SaveObj->Player.Stamina);
	}
	else
	{
		UE_LOG(LogIroncladSave, Warning, TEXT("[Save] Pawn has no VitalsComponent: %s"), *GetNameSafe(Pawn));
	}

	// Cooldowns
	if (UIroncladAbilityComponent* Abilities = Pawn->FindComponentByClass<UIroncladAbilityComponent>())
	{
		Abilities->ApplyCooldownsFromSave(SaveObj->Player.Cooldowns);
	}

	UE_LOG(LogIroncladSave, Display, TEXT("[Save] Applied save to pawn=%s"), *GetNameSafe(Pawn));
	return true;
}

bool UIroncladSaveSubsystem::LoadNow()
{
	if (!HasSave())
	{
		UE_LOG(LogIroncladSave, Warning, TEXT("[Save] No save exists."));
		return false;
	}

	USaveGame* Raw = UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex);
	UIroncladSaveGame* SaveObj = Cast<UIroncladSaveGame>(Raw);
	if (!SaveObj)
	{
		UE_LOG(LogIroncladSave, Warning, TEXT("[Save] LoadGameFromSlot returned invalid object."));
		return false;
	}

	// If pawn isn't ready yet, queue it.
	if (!ApplySaveToCurrentPawn(SaveObj))
	{
		bPendingLoadOnMap = true;
		return true;
	}

	return true;
}

void UIroncladSaveSubsystem::RequestLoadOnNextMap()
{
	bPendingLoadOnMap = true;
}

void UIroncladSaveSubsystem::HandlePostLoadMap(UWorld* LoadedWorld)
{
	if (!bPendingLoadOnMap)
	{
		return;
	}

	bPendingLoadOnMap = false;

	UE_LOG(LogIroncladSave, Display, TEXT("[Save] PostLoadMap fired. Attempting deferred apply..."));

	USaveGame* Raw = UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex);
	UIroncladSaveGame* SaveObj = Cast<UIroncladSaveGame>(Raw);
	if (!SaveObj)
	{
		UE_LOG(LogIroncladSave, Warning, TEXT("[Save] Deferred load failed: invalid save object."));
		return;
	}

	// Try again now that the map is loaded.
	ApplySaveToCurrentPawn(SaveObj);
}