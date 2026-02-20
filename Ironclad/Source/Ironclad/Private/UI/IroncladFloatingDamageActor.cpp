#include "UI/IroncladFloatingDamageActor.h"

#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

#include "Logging/LogMacros.h"

DEFINE_LOG_CATEGORY_STATIC(LogIroncladDamageText, Log, All);

AIroncladFloatingDamageActor::AIroncladFloatingDamageActor()
{
    PrimaryActorTick.bCanEverTick = true; // we need tick for billboard

    WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
    SetRootComponent(WidgetComp);

    WidgetComp->SetWidgetSpace(EWidgetSpace::World);

    // IMPORTANT: do NOT rely on BP for these
    WidgetComp->SetTwoSided(false);
    WidgetComp->SetDrawAtDesiredSize(false);
    WidgetComp->SetDrawSize(FVector2D(200.f, 90.f));

    WidgetComp->SetWorldScale3D(FVector(0.03f));
    WidgetComp->SetPivot(FVector2D(0.5f, 0.5f)); // centers it nicely
}

void AIroncladFloatingDamageActor::BeginPlay()
{
	Super::BeginPlay();

	// Make sure widget instance exists
	if (WidgetComp)
	{
		WidgetComp->InitWidget();
	}

    if (bHasPendingDamage) {
        ApplyDamageToWidget();
    }

	SetLifeSpan(LifeSeconds);
}

void AIroncladFloatingDamageActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!bFaceCamera)
    {
        return;
    }

    APlayerCameraManager* Cam = UGameplayStatics::GetPlayerCameraManager(this, 0);
    if (!Cam)
    {
        return;
    }

    const FVector CamLoc = Cam->GetCameraLocation();
    const FVector MyLoc = GetActorLocation();

    FRotator TargetRot = (CamLoc - MyLoc).Rotation();

    if (bYawOnly)
    {
        TargetRot.Pitch = 0.f;
        TargetRot.Roll = 0.f;
    }

    const FRotator NewRot = FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaSeconds, FaceInterpSpeed);
    SetActorRotation(NewRot);
}

void AIroncladFloatingDamageActor::Init(float DamageAmount)
{
	PendingDamage = DamageAmount;
	bHasPendingDamage = true;

	ApplyDamageToWidget();
}

void AIroncladFloatingDamageActor::ApplyDamageToWidget()
{
    if (!bHasPendingDamage || !WidgetComp) {
        return;
    }

    UUserWidget* W = WidgetComp->GetUserWidgetObject();
    if (!W)
    {
        UE_LOG(LogIroncladDamageText, Warning, TEXT("[DamageFloat] No UserWidgetObject yet on %s"), *GetNameSafe(this));
        return;
    }

    static const FName InitFuncName(TEXT("InitDamage"));
    UFunction* Fn = W->FindFunction(InitFuncName);
    if (!Fn)
    {
        UE_LOG(LogIroncladDamageText, Warning, TEXT("[DamageFloat] Widget %s missing function InitDamage(float)"),
            *GetNameSafe(W));
        return;
    }

    // Allocate an args buffer the exact size BP expects
    uint8* Params = (uint8*)FMemory_Alloca(Fn->ParmsSize);
    FMemory::Memzero(Params, Fn->ParmsSize);

    // Find the first property (the DamageAmount parameter)
    FProperty* Prop = Fn->PropertyLink;
    while (Prop && !(Prop->PropertyFlags & CPF_Parm))
    {
        Prop = Prop->PropertyLinkNext;
    }

    // Or, more explicitly, find by name:
    FProperty* DamageProp = Fn->FindPropertyByName(TEXT("DamageAmount"));
    if (!DamageProp)
    {
        UE_LOG(LogIroncladDamageText, Warning, TEXT("[DamageFloat] InitDamage has no param named DamageAmount"));
        return;
    }

    if (FFloatProperty* FloatProp = CastField<FFloatProperty>(DamageProp))
    {
        FloatProp->SetPropertyValue_InContainer(Params, PendingDamage);
    }
    else if (FDoubleProperty* DoubleProp = CastField<FDoubleProperty>(DamageProp))
    {
        DoubleProp->SetPropertyValue_InContainer(Params, (double)PendingDamage);
    }
    else
    {
        UE_LOG(LogIroncladDamageText, Warning,
            TEXT("[DamageFloat] DamageAmount has unsupported type: %s"),
            *DamageProp->GetClass()->GetName());
        return;
    }

    // Call BP
    W->ProcessEvent(Fn, Params);

    UE_LOG(LogIroncladDamageText, Warning, TEXT("[DamageFloat] InitDamage executed on %s Damage=%.1f"),
        *GetNameSafe(W), PendingDamage);

    bHasPendingDamage = false;
}