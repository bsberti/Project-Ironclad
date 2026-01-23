#pragma once

#include "Characters/IroncladCharacterBase.h"
#include "IroncladDummyEnemy.generated.h"

UCLASS()
class IRONCLAD_API AIroncladDummyEnemy : public AIroncladCharacterBase
{
	GENERATED_BODY()

public:
	AIroncladDummyEnemy();

protected:
	virtual void HandleDeath() override;

	virtual void BeginPlay() override;
};
