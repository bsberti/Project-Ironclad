#pragma once

#include "Characters/IroncladEnemyBase.h"
#include "IroncladControllerEnemy.generated.h"

UCLASS()
class IRONCLAD_API AIroncladControllerEnemy : public AIroncladEnemyBase
{
	GENERATED_BODY()

public:
	AIroncladControllerEnemy();

protected:
	virtual void BeginPlay() override;

public:
	// Tuning (can be set per BP child)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ironclad|AI|Controller")
	float DesiredRange = 700.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ironclad|AI|Controller")
	float RangeTolerance = 250.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ironclad|AI|Controller")
	float ZoneCooldownSeconds = 8.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ironclad|AI|Controller")
	float ZoneTriggerInnerRange = 900.0f;
};
