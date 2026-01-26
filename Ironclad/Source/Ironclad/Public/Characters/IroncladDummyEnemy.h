#pragma once

#include "Characters/IroncladEnemyBase.h"
#include "IroncladDummyEnemy.generated.h"

UCLASS()
class IRONCLAD_API AIroncladDummyEnemy : public AIroncladEnemyBase
{
	GENERATED_BODY()

public:
	AIroncladDummyEnemy();

protected:
	virtual void HandleDeath() override;

	virtual void BeginPlay() override;
};
