#include "GolfOutOfBoundsVolume.h"

#include "GolfBallActor.h"

AGolfOutOfBoundsVolume::AGolfOutOfBoundsVolume() = default;

void AGolfOutOfBoundsVolume::BeginPlay()
{
    Super::BeginPlay();
    OnActorBeginOverlap.AddDynamic(this, &AGolfOutOfBoundsVolume::HandleActorBeginOverlap);
}

void AGolfOutOfBoundsVolume::HandleActorBeginOverlap(AActor *OverlappedActor, AActor *OtherActor)
{
    if (!OtherActor)
    {
        return;
    }

    OnBallOutOfBounds.Broadcast(OtherActor);

    if (bAutoResetIfGolfBall)
    {
        if (AGolfBallActor *Ball = Cast<AGolfBallActor>(OtherActor))
        {
            Ball->ResetToLastSafe(true);
        }
    }
}
