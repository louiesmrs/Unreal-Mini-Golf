#include "BallLockComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GolfBallActor.h"

UBallLockComponent::UBallLockComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UBallLockComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UBallLockComponent::TeleportAndLockToBall(AGolfBallActor *Ball, float Radius, float Height, float Smooth)
{
    if (!Ball)
    {
        return;
    }

    const FTransform T = Ball->GetSuggestedTeleportTransform(Radius, Height);
    if (AActor *Owner = GetOwner())
    {
        Owner->SetActorLocationAndRotation(T.GetLocation(), T.Rotator(), false, nullptr, ETeleportType::TeleportPhysics);
    }

    // face the ball
    if (APawn *OwnerPawn = Cast<APawn>(GetOwner()))
    {
        if (AController *C = OwnerPawn->GetController())
        {
            C->SetControlRotation(T.Rotator());
        }
    }

    LockToBall(Ball, Radius, Smooth);
}

void UBallLockComponent::LockToBall(AGolfBallActor *Ball, float Radius, float Smooth)
{
    if (!Ball)
    {
        return;
    }

    LockedBall = Ball;
    LockCenter = Ball->GetActorLocation();
    LockRadius = Radius;
    LockSmooth = Smooth;
    bLocked = true;

    // optional: orient movement
    if (ACharacter *Char = Cast<ACharacter>(GetOwner()))
    {
        if (UCharacterMovementComponent *CM = Char->GetCharacterMovement())
        {
            CM->bOrientRotationToMovement = true;
        }
    }
    // Broadcast locked event
    OnLocked.Broadcast(Ball);
}

void UBallLockComponent::Unlock()
{
    bLocked = false;
    LockedBall = nullptr;
    // Broadcast unlocked (no ball)
    OnUnlocked.Broadcast(nullptr);
}

FVector UBallLockComponent::GetLockedMovementVector(float ForwardValue, float RightValue) const
{
    if (!bLocked)
    {
        return FVector::ZeroVector;
    }

    const FVector OwnerLoc = GetOwner() ? GetOwner()->GetActorLocation() : FVector::ZeroVector;
    FVector Radial = (LockCenter - OwnerLoc);
    if (Radial.SizeSquared() < KINDA_SMALL_NUMBER)
    {
        // fallback to owner forward/right
        if (AActor *O = GetOwner())
        {
            FVector Dir = O->GetActorForwardVector() * ForwardValue + O->GetActorRightVector() * RightValue;
            if (Dir.IsNearlyZero())
                return FVector::ZeroVector;
            return Dir.GetSafeNormal() * LockedMoveSpeed * FMath::Clamp(FMath::Abs(ForwardValue) + FMath::Abs(RightValue), 0.f, 1.f);
        }
        return FVector::ZeroVector;
    }

    FVector RadialDir = Radial.GetSafeNormal();
    FVector Tangent = FVector::CrossProduct(RadialDir, FVector::UpVector).GetSafeNormal();
    FVector Result = RadialDir * ForwardValue + Tangent * RightValue;
    if (Result.IsNearlyZero())
        return FVector::ZeroVector;
    return Result.GetSafeNormal() * LockedMoveSpeed * FMath::Clamp(Result.Size(), 0.f, 1.f);
}

void UBallLockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!bLocked)
    {
        return;
    }

    AActor *Owner = GetOwner();
    if (!Owner)
    {
        return;
    }

    const FVector PawnLoc = Owner->GetActorLocation();
    const FVector Delta = PawnLoc - LockCenter;
    const FVector2D Delta2D(Delta.X, Delta.Y);
    const float Dist = Delta2D.Size();

    if (Dist <= LockRadius)
    {
        return;
    }

    const FVector2D Dir = Delta2D.GetSafeNormal();
    const FVector2D ClampedXY = Dir * LockRadius;
    const FVector NewLoc(LockCenter.X + ClampedXY.X, LockCenter.Y + ClampedXY.Y, PawnLoc.Z);

    if (LockSmooth <= 0.f)
    {
        Owner->SetActorLocation(NewLoc, false, nullptr, ETeleportType::TeleportPhysics);
    }
    else
    {
        const FVector Interp = FMath::VInterpTo(PawnLoc, NewLoc, DeltaTime, LockSmooth);
        Owner->SetActorLocation(Interp, true);
    }
}
