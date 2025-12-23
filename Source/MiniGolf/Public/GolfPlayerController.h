// GolfPlayerController.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GolfPlayerController.generated.h"

class AGolfBallActor;

UCLASS()
class MINIGOLF_API AGolfPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AGolfPlayerController();

    virtual void PlayerTick(float DeltaSeconds) override;

    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Camera")
    void StartLookAtBall(AGolfBallActor *Ball);

    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Camera")
    void StopLookAtBall();

    UFUNCTION(BlueprintPure, Category = "MiniGolf|Camera")
    FRotator GetDesiredLookAtRot(const FVector &CameraLocation, const FVector &TargetLocation, float HeightOffset = 0.f) const;

    UFUNCTION(BlueprintPure, Category = "MiniGolf|Camera")
    FRotator GetSmoothedRotation(const FRotator &CurrentRot, const FRotator &DesiredRot, float DeltaSeconds, float InterpSpeed) const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGolf|Camera")
    float CameraLookHeight = 60.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGolf|Camera")
    float CameraInterpSpeed = 10.f;

    UPROPERTY()
    TWeakObjectPtr<AGolfBallActor> LookAtBallTarget;

    UPROPERTY()
    bool bCameraLookAtBall = false;

    FVector GetControlledCameraLocation() const;
};
