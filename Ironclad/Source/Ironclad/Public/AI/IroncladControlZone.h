#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IroncladControlZone.generated.h"

class USphereComponent;

UCLASS()
class IRONCLAD_API AIroncladControlZone : public AActor
{
	GENERATED_BODY()

public:
	AIroncladControlZone();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zone")
	TObjectPtr<USphereComponent> Sphere;

	// Gameplay tuning
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Zone|Tuning")
	float Radius = 350.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Zone|Tuning")
	float DurationSeconds = 4.0f;

	// 0.6 = 60% speed, i.e. a slow
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Zone|Tuning", meta = (ClampMin = "0.05", ClampMax = "1.0"))
	float SlowMultiplier = 0.6f;

	// If true, only affects the player pawn.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Zone|Tuning")
	bool bAffectPlayersOnly = true;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	struct FSpeedState
	{
		float OriginalSpeed = 0.0f;
		int32 StackCount = 0;
	};

	// Supports stacking safely (multiple zones overlapping).
	TMap<TWeakObjectPtr<class ACharacter>, FSpeedState> Affected;
	void ApplySlow(class ACharacter* Char);
	void RemoveSlow(class ACharacter* Char);
	void RestoreAll();
};
