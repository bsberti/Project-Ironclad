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

	void Init(float DamageAmount);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:

	// Camera-facing
	UPROPERTY(EditDefaultsOnly, Category = "FloatingDamage|Billboard")
	bool bFaceCamera = true;

	UPROPERTY(EditDefaultsOnly, Category = "FloatingDamage|Billboard")
	bool bYawOnly = true;

	UPROPERTY(EditDefaultsOnly, Category = "FloatingDamage|Billboard")
	float FaceInterpSpeed = 20.f;

private:
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UWidgetComponent> WidgetComp;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	float LifeSeconds = 2.0f;

	float PendingDamage = 0.f;
	bool bHasPendingDamage = false;

	void ApplyDamageToWidget();
};
