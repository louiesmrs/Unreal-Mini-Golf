// GolfPlayerController.cpp
#include "GolfPlayerController.h"
#include "../GolfBallActor.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AGolfPlayerController::AGolfPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AGolfPlayerController::PlayerTick(float DeltaSeconds)
{
    Super::PlayerTick(DeltaSeconds);

    if (bCameraLookAtBall && LookAtBallTarget.IsValid())
    {
        FVector CamLoc = GetControlledCameraLocation();
        FVector TargetLoc = LookAtBallTarget->GetActorLocation();
        FRotator Desired = GetDesiredLookAtRot(CamLoc, TargetLoc, CameraLookHeight);
        FRotator NewRot = GetSmoothedRotation(GetControlRotation(), Desired, DeltaSeconds, CameraInterpSpeed);
        SetControlRotation(NewRot);
    }
}

void AGolfPlayerController::StartLookAtBall(AGolfBallActor *Ball)
{
    LookAtBallTarget = Ball;
    bCameraLookAtBall = Ball != nullptr;
}

void AGolfPlayerController::StopLookAtBall()
{
    bCameraLookAtBall = false;
    LookAtBallTarget = nullptr;
}

FRotator AGolfPlayerController::GetDesiredLookAtRot(const FVector &CameraLocation, const FVector &TargetLocation, float HeightOffset) const
{
    FVector Target = TargetLocation + FVector(0.f, 0.f, HeightOffset);
    return (Target - CameraLocation).Rotation();
}

FRotator AGolfPlayerController::GetSmoothedRotation(const FRotator &CurrentRot, const FRotator &DesiredRot, float DeltaSeconds, float InterpSpeed) const
{
    return FMath::RInterpTo(CurrentRot, DesiredRot, DeltaSeconds, InterpSpeed);
}

FVector AGolfPlayerController::GetControlledCameraLocation() const
{
    if (APawn *P = GetPawn())
    {
        if (UCameraComponent *Cam = P->FindComponentByClass<UCameraComponent>())
            return Cam->GetComponentLocation();
        if (USpringArmComponent *Arm = P->FindComponentByClass<USpringArmComponent>())
            return Arm->GetComponentLocation();
        return P->GetActorLocation();
    }
    return FVector::ZeroVector;
}
