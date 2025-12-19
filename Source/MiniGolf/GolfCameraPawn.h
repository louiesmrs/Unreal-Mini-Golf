#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GolfCameraPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USceneComponent;
class AGolfBallActor;

/**
 * Dedicated camera pawn for mini-golf.
 * For now it focuses on teleporting to a good aiming position relative to the ball.
 */
UCLASS(BlueprintType)
class MINIGOLF_API AGolfCameraPawn : public APawn
{
    GENERATED_BODY()

public:
    AGolfCameraPawn();

    UFUNCTION(BlueprintPure, Category = "MiniGolf|Camera")
    UCameraComponent *GetCamera() const { return Camera; }

    UFUNCTION(BlueprintPure, Category = "MiniGolf|Camera")
    USpringArmComponent *GetSpringArm() const { return SpringArm; }

    /**
     * Teleports this pawn to the ball's suggested aiming transform.
     * Also sets the possessing controller's control rotation to face the ball.
     */
    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Camera")
    void TeleportToBallSuggested(AGolfBallActor *Ball, float Distance = -1.f, float Height = -1.f);

    /** Sets which ball this camera should orbit around. */
    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Camera")
    void SetTargetBall(AGolfBallActor *Ball);

    /** Adds orbit input. Use MouseX as YawDelta; PitchDelta is optional. */
    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Camera")
    void AddOrbitInput(float YawDelta, float PitchDelta = 0.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGolf|Camera")
    float DefaultDistance = 350.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGolf|Camera")
    float DefaultHeight = 80.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGolf|Camera")
    float OrbitYawSpeed = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGolf|Camera")
    float OrbitPitchSpeed = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGolf|Camera")
    float OrbitPitchMin = -80.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGolf|Camera")
    float OrbitPitchMax = -5.f;

protected:
    virtual void BeginPlay() override;

private:
    void UpdateOrbitTransform();

private:
    UPROPERTY(VisibleAnywhere, Category = "MiniGolf|Components")
    TObjectPtr<USceneComponent> Root;

    UPROPERTY(VisibleAnywhere, Category = "MiniGolf|Components")
    TObjectPtr<USpringArmComponent> SpringArm;

    UPROPERTY(VisibleAnywhere, Category = "MiniGolf|Components")
    TObjectPtr<UCameraComponent> Camera;

    UPROPERTY(Transient)
    TObjectPtr<AGolfBallActor> TargetBall;

    float OrbitYawDegrees = 0.f;
    float OrbitPitchDegrees = -20.f;
    float OrbitDistance = 350.f;
    float OrbitHeight = 80.f;
};
