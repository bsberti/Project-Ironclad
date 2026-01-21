#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IroncladHitDetectionComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogIroncladHitDetect, Log, All);

USTRUCT(BlueprintType)
struct FIroncladTraceConfig
{
	GENERATED_BODY()

	// Socket names on the TraceSource component (usually the weapon mesh).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitDetection")
	FName StartSocket = NAME_None;   // e.g. "weapon_base"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitDetection")
	FName EndSocket = NAME_None;     // e.g. "weapon_tip"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitDetection", meta = (ClampMin = "0.1"))
	float Radius = 10.0f;

	// Recommend: create Project Settings → Collision → Trace Channel "WeaponTrace"
	// and map it to a GameTraceChannel (e.g., ECC_GameTraceChannel1).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitDetection")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_GameTraceChannel1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitDetection")
	bool bTraceComplex = false;

	// Per-attack debug draw.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitDetection")
	bool bDebugDraw = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitDetection", meta = (ClampMin = "0.0"))
	float DebugDrawTime = 0.5f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponHitDetected, const FHitResult&, Hit);

UCLASS(ClassGroup = (Ironclad), meta = (BlueprintSpawnableComponent))
class IRONCLAD_API UIroncladHitDetectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UIroncladHitDetectionComponent();

	// Starts a hit window. Traces will run only while active.
	UFUNCTION(BlueprintCallable, Category = "HitDetection")
	void BeginHitWindow(USceneComponent* InTraceSource, const FIroncladTraceConfig& InConfig);

	UFUNCTION(BlueprintCallable, Category = "HitDetection")
	void EndHitWindow();

	UFUNCTION(BlueprintCallable, Category = "HitDetection")
	bool IsHitWindowActive() const { return bWindowActive; }

	// Global debug toggle (in addition to config.bDebugDraw).
	UFUNCTION(BlueprintCallable, Category = "HitDetection")
	void SetDebugEnabled(bool bEnabled) { bDebugEnabled = bEnabled; }

	UFUNCTION(BlueprintCallable, Category = "HitDetection")
	bool IsDebugEnabled() const { return bDebugEnabled; }

	UPROPERTY(BlueprintAssignable, Category = "HitDetection")
	FOnWeaponHitDetected OnHitDetected;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool bWindowActive = false;
	bool bDebugEnabled = false;

	UPROPERTY(Transient)
	TObjectPtr<USceneComponent> TraceSource = nullptr;

	FIroncladTraceConfig ActiveConfig;

	bool bHasPrev = false;
	FVector PrevStart = FVector::ZeroVector;
	FVector PrevEnd = FVector::ZeroVector;

	// Prevent multi-hits on the same actor per window.
	TSet<TWeakObjectPtr<AActor>> HitActorsThisWindow;

	FVector GetSocketWorldLocationSafe(const FName SocketName) const;

	void ResetPrev();
	void ProcessSweeps(const FVector& CurrStart, const FVector& CurrEnd);
	void HandleHitResult(const FHitResult& Hit);

	void DebugDrawFrame(const FVector& CurrStart, const FVector& CurrEnd) const;
};