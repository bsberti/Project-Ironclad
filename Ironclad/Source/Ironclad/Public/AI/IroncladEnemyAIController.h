#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "IroncladEnemyAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;

UCLASS()
class IRONCLAD_API AIroncladEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AIroncladEnemyAIController();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	TObjectPtr<UAIPerceptionComponent> AIPerception;

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;

	/** Track current target (we’ll keep it simple: just the player) */
	UPROPERTY(VisibleInstanceOnly, Category = "AI|Perception")
	TObjectPtr<AActor> CurrentTarget;

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	void SetCurrentTarget(AActor* NewTarget, const FString& Reason);
	void ClearCurrentTarget(const FString& Reason);
};
