#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GolfBallActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBallRestStateChanged, bool, bIsResting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBallReset, const FTransform &, NewTransform);

UCLASS(BlueprintType)
class MINIGOLF_API AGolfBallActor : public AActor
{
    GENERATED_BODY()

public:
    AGolfBallActor();
    virtual void Tick(float DeltaSeconds) override;

    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Ball")
    void ApplyHitImpulse(const FVector &WorldDirection, float Strength);

    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Ball")
    void SetLastHitTransform(const FTransform &InTransform);

    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Ball")
    void MarkCurrentAsSafe();

    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Ball")
    void SetSafeTransform(const FTransform &InTransform);

    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Ball")
    void ResetToLastSafe(bool bZeroVelocity = true);

    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Ball")
    void ResetToTransform(const FTransform &InTransform, bool bZeroVelocity = true);

    UFUNCTION(BlueprintPure, Category = "MiniGolf|Ball")
    bool IsResting() const { return bIsResting; }

    UFUNCTION(BlueprintPure, Category = "MiniGolf|Ball")
    FTransform GetLastSafeTransform() const { return LastSafeTransform; }

    UFUNCTION(BlueprintPure, Category = "MiniGolf|Ball")
    FTransform GetLastHitTransform() const { return LastHitTransform; }

    /**
     * Returns a suggested transform for teleporting a player to a good aiming position relative to the ball.
     * Uses the ball's last hit rotation (if available) to place the point behind the ball.
     * Distance is how far behind the ball (XY), Height is Z offset above the ball.
     */
    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Ball")
    FTransform GetSuggestedTeleportTransform(float Distance = 350.f, float Height = 80.f) const;

    UFUNCTION(BlueprintPure, Category = "MiniGolf|Ball")
    USphereComponent *GetCollision() const { return Collision; }

public:
    UPROPERTY(BlueprintAssignable, Category = "MiniGolf|Events")
    FOnBallRestStateChanged OnRestStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "MiniGolf|Events")
    FOnBallReset OnBallReset;

protected:
    virtual void BeginPlay() override;

private:
    void UpdateRestState(float DeltaSeconds);
    void SetResting(bool bNewResting);

private:
    UPROPERTY(VisibleAnywhere, Category = "MiniGolf|Components")
    TObjectPtr<USphereComponent> Collision;

    UPROPERTY(VisibleAnywhere, Category = "MiniGolf|Components")
    TObjectPtr<UStaticMeshComponent> Visual;

    UPROPERTY(EditAnywhere, Category = "MiniGolf|Tuning")
    float RestingSpeedThreshold = 15.0f;

    UPROPERTY(EditAnywhere, Category = "MiniGolf|Tuning")
    float RestingTimeSeconds = 0.15f;

    UPROPERTY(VisibleAnywhere, Category = "MiniGolf|State")
    bool bIsResting = false;

    float RestingAccumulator = 0.0f;

    UPROPERTY(VisibleAnywhere, Category = "MiniGolf|State")
    FTransform LastSafeTransform;

    UPROPERTY(VisibleAnywhere, Category = "MiniGolf|State")
    FTransform LastHitTransform;
};
