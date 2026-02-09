#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"

#include <BehaviorTree/BehaviorTreeComponent.h>

#include "IroncladEnemyAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UBlackboardComponent;
class UBehaviorTree;
class UBehaviorTreeComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogIroncladPerception, Log, All);

UCLASS()
class IRONCLAD_API AIroncladEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AIroncladEnemyAIController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;

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
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UBlackboardComponent* BlackboardComp = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UBehaviorTreeComponent* BehaviorTreeComp = nullptr;

private:
	// --- Debug helpers ---
	void DrawAIDebug() const;
	FString BuildDebugText() const;

	bool BlackboardHasKey(const FName& KeyName) const;
};
