#pragma once

#include "Characters/IroncladCharacterBase.h"
#include "IroncladEnemyBase.generated.h"

UENUM(BlueprintType)
enum class EIroncladFaction : uint8
{
	Hostile UMETA(DisplayName = "Hostile"),
	Neutral UMETA(DisplayName = "Neutral"),
	Friendly UMETA(DisplayName = "Friendly"),
};

UCLASS(Abstract)
class IRONCLAD_API AIroncladEnemyBase : public AIroncladCharacterBase
{
	GENERATED_BODY()

public:
	AIroncladEnemyBase();

	// Enemy identity (used later by targeting/AI rules)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ironclad|Enemy")
	EIroncladFaction Faction = EIroncladFaction::Hostile;

protected:
	virtual void OnHitReceived_Implementation();

	// Override shared death baseline to allow enemy-specific policy
	virtual void HandleDeath() override;

	virtual void BeginPlay() override;
};
