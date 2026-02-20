#include "Characters/IroncladEnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/IroncladVitalsComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogIroncladEnemyBase, Log, All);

AIroncladEnemyBase::AIroncladEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	EnemyHealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHealthBarWidget"));
	EnemyHealthBarWidget->SetupAttachment(GetRootComponent());

	EnemyHealthBarWidget->SetWidgetSpace(EWidgetSpace::World);
	EnemyHealthBarWidget->SetDrawSize(FVector2D(100.f, 10.f));
	EnemyHealthBarWidget->SetPivot(FVector2D(0.5f, 0.5f));
	EnemyHealthBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 160.f));

	// Start hidden; we show on damage.
	EnemyHealthBarWidget->SetVisibility(false);
}

void AIroncladEnemyBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!EnemyHealthBarWidget)
	{
		return;
	}

	APlayerCameraManager* Cam = UGameplayStatics::GetPlayerCameraManager(this, 0);
	if (!Cam)
	{
		return;
	}

	const FVector CamLoc = Cam->GetCameraLocation();
	const FVector WidgetLoc = EnemyHealthBarWidget->GetComponentLocation();

	const FRotator LookAt = (CamLoc - WidgetLoc).Rotation();

	EnemyHealthBarWidget->SetWorldRotation(FRotator(0.f, LookAt.Yaw, 0.f));
}

void AIroncladEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if (EnemyHealthBarWidgetClass)
	{
		EnemyHealthBarWidget->SetWidgetClass(EnemyHealthBarWidgetClass);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemyUI] EnemyHealthBarWidgetClass not set on %s"), *GetName());
	}
	
	CachedVitals = FindComponentByClass<UIroncladVitalsComponent>();
	if (!CachedVitals)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemyUI] No Vitals component on %s"), *GetName());
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[EnemyUI] BeginPlay %s WidgetClass=%s WidgetComp=%s"),
		*GetName(), *GetNameSafe(EnemyHealthBarWidgetClass), *GetNameSafe(EnemyHealthBarWidget));

	// Initialize last health + push initial UI (still hidden).
	LastHealth = CachedVitals->GetHealth();
	UpdateHealthBar(LastHealth, CachedVitals->GetMaxHealth());

	// Bind health changes
	CachedVitals->OnHealthChanged.AddDynamic(this, &AIroncladEnemyBase::HandleHealthChanged);

	UE_LOG(LogTemp, Warning, TEXT("[EnemyUI] Bound OnHealthChanged for %s Vitals=%s Health=%.1f"),
		*GetName(), *GetNameSafe(CachedVitals), CachedVitals ? CachedVitals->GetHealth() : -1.f);
}

void AIroncladEnemyBase::HandleHealthChanged(float Current, float Max)
{
	UE_LOG(LogTemp, Warning, TEXT("[EnemyUI] HealthChanged %s: %.1f -> %.1f (Max=%.1f) TookDamage=%d"),
		*GetName(), LastHealth, Current, Max, (Current < LastHealth - KINDA_SMALL_NUMBER));

	// Detect damage (health decreased).
	const bool bTookDamage = (Current < LastHealth - KINDA_SMALL_NUMBER);
	LastHealth = Current;

	UpdateHealthBar(Current, Max);

	if (bTookDamage)
	{
		ShowHealthBar(Current, Max);
	}
}

void AIroncladEnemyBase::UpdateHealthBar(float Current, float Max)
{
	if (!EnemyHealthBarWidget)
	{
		return;
	}

	UIroncladEnemyHealthBarWidget* W =
		Cast<UIroncladEnemyHealthBarWidget>(EnemyHealthBarWidget->GetUserWidgetObject());

	if (!W)
	{
		return;
	}

	W->SetHealth(Current, Max);
}

void AIroncladEnemyBase::ShowHealthBar(float Current, float Max)
{
	if (!EnemyHealthBarWidget)
	{
		return;
	}

	EnemyHealthBarWidget->SetVisibility(true);

	UE_LOG(LogTemp, Warning, TEXT("[EnemyUI] ShowHealthBar %s (making visible)"), *GetName());

	// Reset hide timer
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(HealthBarHideTimer);
		World->GetTimerManager().SetTimer(HealthBarHideTimer, this, &AIroncladEnemyBase::HideHealthBar, HealthBarHideDelay, false);
	}
}

void AIroncladEnemyBase::HideHealthBar()
{
	if (EnemyHealthBarWidget)
	{
		EnemyHealthBarWidget->SetVisibility(false);
	}
}

void AIroncladEnemyBase::HandleDeath()
{
	// Keep shared baseline behavior centralized in CharacterBase
	Super::HandleDeath();

	// Enemy-level hook point (keep minimal for now)
	UE_LOG(LogIroncladEnemyBase, Display, TEXT("[EnemyBase] %s died."), *GetName());
}

void AIroncladEnemyBase::OnHitReceived_Implementation()
{
	// Placeholder for Card 3.7
}