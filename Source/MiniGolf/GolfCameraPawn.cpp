#include "GolfCameraPawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "GolfBallActor.h"

AGolfCameraPawn::AGolfCameraPawn()
{
    PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(Root);
    SpringArm->TargetArmLength = 0.0f;
    SpringArm->bDoCollisionTest = false;
    SpringArm->bUsePawnControlRotation = false;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

    bUseControllerRotationYaw = true;
    bUseControllerRotationPitch = true;
    bUseControllerRotationRoll = false;

    AutoPossessPlayer = EAutoReceiveInput::Disabled;
}

void AGolfCameraPawn::BeginPlay()
{
    Super::BeginPlay();
}

void AGolfCameraPawn::TeleportToBallSuggested(AGolfBallActor *Ball, float Distance, float Height)
{
    if (!Ball)
    {
        return;
    }

    const float UseDistance = (Distance > 0.f) ? Distance : DefaultDistance;
    const float UseHeight = (Height > 0.f) ? Height : DefaultHeight;

    SetTargetBall(Ball);

    const FTransform T = Ball->GetSuggestedTeleportTransform(UseDistance, UseHeight);

    OrbitDistance = FVector::Dist2D(Ball->GetActorLocation(), T.GetLocation());
    OrbitHeight = T.GetLocation().Z - Ball->GetActorLocation().Z;

    const FRotator LookAt = T.Rotator();
    OrbitYawDegrees = LookAt.Yaw;
    OrbitPitchDegrees = FMath::ClampAngle(LookAt.Pitch, OrbitPitchMin, OrbitPitchMax);

    UpdateOrbitTransform();
}

void AGolfCameraPawn::SetTargetBall(AGolfBallActor *Ball)
{
    TargetBall = Ball;
}

void AGolfCameraPawn::AddOrbitInput(float YawDelta, float PitchDelta)
{
    if (!TargetBall)
    {
        return;
    }

    OrbitYawDegrees += (YawDelta * OrbitYawSpeed);
    OrbitPitchDegrees = FMath::ClampAngle(OrbitPitchDegrees + (PitchDelta * OrbitPitchSpeed), OrbitPitchMin, OrbitPitchMax);

    UpdateOrbitTransform();
}

void AGolfCameraPawn::UpdateOrbitTransform()
{
    if (!TargetBall)
    {
        return;
    }

    const FVector BallLocation = TargetBall->GetActorLocation();
    const FVector YawForward = FRotator(0.f, OrbitYawDegrees, 0.f).Vector();

    const FVector CameraLocation = BallLocation - (YawForward * OrbitDistance) + FVector(0.f, 0.f, OrbitHeight);
    const FRotator LookAt = (BallLocation - CameraLocation).Rotation();
    const FRotator FinalRotation(OrbitPitchDegrees, LookAt.Yaw, 0.f);

    SetActorLocationAndRotation(
        CameraLocation,
        FinalRotation,
        false,
        nullptr,
        ETeleportType::TeleportPhysics);

    if (AController *C = GetController())
    {
        C->SetControlRotation(FinalRotation);
    }
}
