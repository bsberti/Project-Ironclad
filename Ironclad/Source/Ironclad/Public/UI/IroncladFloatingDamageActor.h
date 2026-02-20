#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IroncladFloatingDamageActor.generated.h"

class UWidgetComponent;

UCLASS()
class IRONCLAD_API AIroncladFloatingDamageActor : public AActor
{
	GENERATED_BODY()

public:
	AIroncladFloatingDamageActor();

	// Call right after spawning
	void Init(float DamageAmount);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UWidgetComponent> WidgetComp;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	float LifeSeconds = 1.0f;
};
