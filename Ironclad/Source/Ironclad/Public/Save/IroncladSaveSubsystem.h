#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "IroncladSaveSubsystem.generated.h"

class UIroncladSaveGame;

DECLARE_LOG_CATEGORY_EXTERN(LogIroncladSave, Log, All);

UCLASS()
class IRONCLAD_API UIroncladSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "Ironclad|Save")
	bool SaveNow();

	UFUNCTION(BlueprintCallable, Category = "Ironclad|Save")
	bool LoadNow();

	UFUNCTION(BlueprintCallable, Category = "Ironclad|Save")
	bool HasSave() const;

	// Optional: call this if you load during startup and want to apply after map/pawn exists.
	UFUNCTION(BlueprintCallable, Category = "Ironclad|Save")
	void RequestLoadOnNextMap();

private:
	UPROPERTY()
	FString SlotName = TEXT("IroncladSlot");

	UPROPERTY()
	int32 UserIndex = 0;

	UPROPERTY()
	TSubclassOf<UIroncladSaveGame> SaveGameClass;

	bool bPendingLoadOnMap = false;

	UIroncladSaveGame* BuildSaveObject() const;
	bool ApplySaveToCurrentPawn(const UIroncladSaveGame* SaveObj) const;

	void HandlePostLoadMap(UWorld* LoadedWorld);

	bool GetLocalPlayerPawn(APawn*& OutPawn) const;
};