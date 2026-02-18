#include "UI/IroncladHUDSubsystem.h"

#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Components/IroncladAbilityComponent.h"

#include "UI/IroncladHUDWidget.h"
#include "Components/IroncladVitalsComponent.h"

void UIroncladHUDSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const FSoftClassPath HUDClassPath(TEXT("/Game/Ironclad/UI/WBP_IroncladHUD.WBP_IroncladHUD_C"));
	HUDWidgetClass = HUDClassPath.TryLoadClass<UIroncladHUDWidget>();

	if (!HUDWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[HUD] Failed to load HUD widget class from: %s"), *HUDClassPath.ToString());
		return;
	}

	EnsureHUD();
}

void UIroncladHUDSubsystem::Deinitialize()
{
	UnbindVitals();
	UnbindAbilities();

	if (HUDWidget)
	{
		HUDWidget->RemoveFromParent();
		HUDWidget = nullptr;
	}

	if (APlayerController* PC = CachedPC.Get())
	{
		PC->OnPossessedPawnChanged.RemoveDynamic(this, &UIroncladHUDSubsystem::HandlePawnChanged);
	}

	CachedPC.Reset();

	Super::Deinitialize();
}

void UIroncladHUDSubsystem::EnsureHUD()
{
	TryBindToLocalPlayer();
}

void UIroncladHUDSubsystem::TryBindToLocalPlayer()
{
	if (!GetWorld())
	{
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("[HUD] TryBindToLocalPlayer World=%s"), *GetNameSafe(GetWorld()));

	UGameInstance* GI = GetGameInstance();
	if (!GI)
	{
		return;
	}

	APlayerController* PC = GI->GetFirstLocalPlayerController(GetWorld());
	if (!PC)
	{
		// World exists but PC not yet created; calling EnsureHUD() again later is fine.
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("[HUD] Local PC=%s Pawn=%s"), *GetNameSafe(PC), *GetNameSafe(PC->GetPawn()));

	if (CachedPC.Get() != PC)
	{
		CachedPC = PC;

		// Create widget (once per local player)
		CreateHUDWidgetIfNeeded(PC);

		// Bind to pawn changes
		// UE5: OnPossessedPawnChanged exists on APlayerController
		PC->OnPossessedPawnChanged.AddDynamic(this, &UIroncladHUDSubsystem::HandlePawnChanged);

		// Bind to current pawn immediately
		HandlePawnChanged(nullptr, PC->GetPawn());
	}
}

void UIroncladHUDSubsystem::CreateHUDWidgetIfNeeded(APlayerController* PC)
{
	if (HUDWidget || !PC)
	{
		return;
	}

	if (!HUDWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[HUD] HUDWidgetClass not set/loaded. Check WBP path."));
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("[HUD] HUDWidgetClass=%s"), *GetNameSafe(HUDWidgetClass));
	//UE_LOG(LogTemp, Warning, TEXT("[HUD] Creating HUD widget..."));

	HUDWidget = CreateWidget<UIroncladHUDWidget>(PC, HUDWidgetClass);
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
	}
}

void UIroncladHUDSubsystem::HandlePawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	UnbindVitals();
	UnbindAbilities();

	BindVitalsFromPawn(NewPawn);
	BindAbilitiesFromPawn(NewPawn);
}

void UIroncladHUDSubsystem::BindVitalsFromPawn(APawn* Pawn)
{
	if (!Pawn)
	{
		return;
	}

	UIroncladVitalsComponent* Vitals = Pawn->FindComponentByClass<UIroncladVitalsComponent>();
	if (!Vitals)
	{
		UE_LOG(LogTemp, Warning, TEXT("[HUD] No IroncladVitalsComponent found on pawn %s"), *Pawn->GetName());
		return;
	}

	BoundVitals = Vitals;

	BoundVitals->OnHealthChanged.AddDynamic(this, &UIroncladHUDSubsystem::HandleHealthChanged);
	BoundVitals->OnStaminaChanged.AddDynamic(this, &UIroncladHUDSubsystem::HandleStaminaChanged);
	BoundVitals->OnDeath.AddDynamic(this, &UIroncladHUDSubsystem::HandleDeath);

	// Push initial state immediately (no reliance on timing/order).
	HandleHealthChanged(BoundVitals->GetHealth(), BoundVitals->GetMaxHealth());
	HandleStaminaChanged(BoundVitals->GetStamina(), BoundVitals->GetMaxStamina());
}

void UIroncladHUDSubsystem::UnbindVitals()
{
	if (!BoundVitals)
	{
		return;
	}

	BoundVitals->OnHealthChanged.RemoveDynamic(this, &UIroncladHUDSubsystem::HandleHealthChanged);
	BoundVitals->OnStaminaChanged.RemoveDynamic(this, &UIroncladHUDSubsystem::HandleStaminaChanged);
	BoundVitals->OnDeath.RemoveDynamic(this, &UIroncladHUDSubsystem::HandleDeath);

	BoundVitals = nullptr;
}

void UIroncladHUDSubsystem::HandleHealthChanged(float Current, float Max)
{
	if (HUDWidget)
	{
		HUDWidget->SetHealth(Current, Max);
	}
}

void UIroncladHUDSubsystem::HandleStaminaChanged(float Current, float Max)
{
	if (HUDWidget)
	{
		HUDWidget->SetStamina(Current, Max);
	}
}

void UIroncladHUDSubsystem::HandleDeath()
{
	// For now, nothing special. Later we can trigger a death overlay, fade, etc.
}

void UIroncladHUDSubsystem::BindAbilitiesFromPawn(APawn* Pawn)
{
	if (!Pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("[HUD] BindAbilitiesFromPawn: Pawn is null"));
		return;
	}

	UIroncladAbilityComponent* Abilities = Pawn->FindComponentByClass<UIroncladAbilityComponent>();
	UE_LOG(LogTemp, Warning, TEXT("[HUD] BindAbilitiesFromPawn: Pawn=%s Abilities=%s"),
		*GetNameSafe(Pawn), *GetNameSafe(Abilities));
	if (!Abilities)
	{
		// Not fatal: some pawns may not have abilities.
		// UE_LOG(LogTemp, Warning, TEXT("[HUD] No IroncladAbilityComponent found on pawn %s"), *Pawn->GetName());
		return;
	}

	BoundAbilities = Abilities;
	UE_LOG(LogTemp, Warning, TEXT("[HUD] Binding cooldown delegates..."));
	BoundAbilities->OnCooldownStarted.AddDynamic(this, &UIroncladHUDSubsystem::HandleCooldownStarted);
	BoundAbilities->OnCooldownEnded.AddDynamic(this, &UIroncladHUDSubsystem::HandleCooldownEnded);

	// Optional: if you want HUD to show cooldowns already active on respawn/load,
	// add a method on AbilityComponent to enumerate active cooldowns and call it here.
}

void UIroncladHUDSubsystem::UnbindAbilities()
{
	if (!BoundAbilities)
	{
		return;
	}

	BoundAbilities->OnCooldownStarted.RemoveDynamic(this, &UIroncladHUDSubsystem::HandleCooldownStarted);
	BoundAbilities->OnCooldownEnded.RemoveDynamic(this, &UIroncladHUDSubsystem::HandleCooldownEnded);

	BoundAbilities = nullptr;
}

void UIroncladHUDSubsystem::HandleCooldownStarted(FName AbilityId, float Duration, double EndTime)
{
	UE_LOG(LogTemp, Warning, TEXT("[HUD] CooldownStarted received: %s Dur=%.2f End=%.2f"),
		*AbilityId.ToString(), Duration, EndTime);

	if (HUDWidget)
	{
		// We’ll treat Remaining as Duration at start; widget can track EndTime internally if desired.
		// If you prefer: change the widget API to accept EndTime too.
		const float Remaining = Duration;
		HUDWidget->SetAbilityCooldown(AbilityId, Remaining, Duration);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[HUD] HUDWidget is null during CooldownStarted"));
	}
}

void UIroncladHUDSubsystem::HandleCooldownEnded(FName AbilityId)
{
	if (HUDWidget)
	{
		HUDWidget->ClearAbilityCooldown(AbilityId);
	}
}
