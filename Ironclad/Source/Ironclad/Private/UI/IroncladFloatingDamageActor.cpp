#include "UI/IroncladFloatingDamageActor.h"

#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"

AIroncladFloatingDamageActor::AIroncladFloatingDamageActor()
{
	PrimaryActorTick.bCanEverTick = false;

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	SetRootComponent(WidgetComp);

	WidgetComp->SetWidgetSpace(EWidgetSpace::World);
	WidgetComp->SetDrawAtDesiredSize(true);
	WidgetComp->SetTwoSided(true);

	// Optional: tune size/scale later
	WidgetComp->SetDrawSize(FVector2D(200.f, 80.f));
}

void AIroncladFloatingDamageActor::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSeconds);
}

void AIroncladFloatingDamageActor::Init(float DamageAmount)
{
	if (UUserWidget* W = WidgetComp ? WidgetComp->GetUserWidgetObject() : nullptr)
	{
		// Call BP function by name OR use a typed base widget (recommended below)
		static const FName InitFuncName(TEXT("InitDamage"));
		if (UFunction* Fn = W->FindFunction(InitFuncName))
		{
			struct FArgs { float DamageAmount; };
			FArgs Args{ DamageAmount };
			W->ProcessEvent(Fn, &Args);
		}
	}
}
