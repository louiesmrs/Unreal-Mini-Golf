#include "GolfBallActor.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

AGolfBallActor::AGolfBallActor()
{
    PrimaryActorTick.bCanEverTick = true;

    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    SetRootComponent(Collision);
    Collision->InitSphereRadius(11.0f);
    Collision->SetSimulatePhysics(true);
    Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Collision->SetCollisionObjectType(ECC_PhysicsBody);
    Collision->SetNotifyRigidBodyCollision(false);

    Visual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual"));
    Visual->SetupAttachment(Collision);
    Visual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGolfBallActor::BeginPlay()
{
    Super::BeginPlay();
    LastSafeTransform = GetActorTransform();
    LastHitTransform = GetActorTransform();
    SetResting(true);
}

void AGolfBallActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    UpdateRestState(DeltaSeconds);
}

void AGolfBallActor::ApplyHitImpulse(const FVector &WorldDirection, float Strength)
{
    if (!Collision || WorldDirection.IsNearlyZero() || Strength <= 0.0f)
    {
        return;
    }

    const FVector Dir = WorldDirection.GetSafeNormal();
    Collision->AddImpulse(Dir * Strength, NAME_None, true);
    SetResting(false);
    RestingAccumulator = 0.0f;
}

void AGolfBallActor::SetLastHitTransform(const FTransform &InTransform)
{
    LastHitTransform = InTransform;
}

void AGolfBallActor::MarkCurrentAsSafe()
{
    LastSafeTransform = GetActorTransform();
}

void AGolfBallActor::SetSafeTransform(const FTransform &InTransform)
{
    LastSafeTransform = InTransform;
}

void AGolfBallActor::ResetToLastSafe(bool bZeroVelocity)
{
    ResetToTransform(LastSafeTransform, bZeroVelocity);
}

void AGolfBallActor::ResetToTransform(const FTransform &InTransform, bool bZeroVelocity)
{
    SetActorTransform(InTransform, false, nullptr, ETeleportType::TeleportPhysics);
    if (Collision && bZeroVelocity)
    {
        Collision->SetPhysicsLinearVelocity(FVector::ZeroVector);
        Collision->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
    }
    SetResting(true);
    RestingAccumulator = 0.0f;
    OnBallReset.Broadcast(InTransform);
}

void AGolfBallActor::UpdateRestState(float DeltaSeconds)
{
    if (!Collision)
    {
        return;
    }

    const float Speed = Collision->GetPhysicsLinearVelocity().Size();
    if (Speed <= RestingSpeedThreshold)
    {
        RestingAccumulator += DeltaSeconds;
        if (RestingAccumulator >= RestingTimeSeconds)
        {
            SetResting(true);
        }
    }
    else
    {
        RestingAccumulator = 0.0f;
        SetResting(false);
    }
}

void AGolfBallActor::SetResting(bool bNewResting)
{
    if (bIsResting == bNewResting)
    {
        return;
    }
    bIsResting = bNewResting;
    OnRestStateChanged.Broadcast(bIsResting);
}

FTransform AGolfBallActor::GetSuggestedTeleportTransform(float Distance, float Height) const
{
    const FVector BallLocation = GetActorLocation();

    FVector Forward = LastHitTransform.GetRotation().GetForwardVector();
    Forward.Z = 0.f;
    if (!Forward.Normalize())
    {
        Forward = GetActorForwardVector();
        Forward.Z = 0.f;
        if (!Forward.Normalize())
        {
            Forward = FVector::ForwardVector;
        }
    }

    const FVector CameraLocation = BallLocation - (Forward * Distance) + FVector(0.f, 0.f, Height);
    const FRotator LookAt = (BallLocation - CameraLocation).Rotation();
    return FTransform(LookAt, CameraLocation);
}
