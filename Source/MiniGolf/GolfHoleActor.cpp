#include "GolfHoleActor.h"

#include "Components/SphereComponent.h"

AGolfHoleActor::AGolfHoleActor()
{
    PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
    Trigger->SetupAttachment(Root);
    Trigger->InitSphereRadius(18.0f);
    Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Trigger->SetCollisionObjectType(ECC_WorldDynamic);
    Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    Trigger->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
    Trigger->SetGenerateOverlapEvents(true);
}

void AGolfHoleActor::BeginPlay()
{
    Super::BeginPlay();
    if (Trigger)
    {
        Trigger->OnComponentBeginOverlap.AddDynamic(this, &AGolfHoleActor::HandleBeginOverlap);
    }
}

void AGolfHoleActor::HandleBeginOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor,
                                        UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    if (!OtherActor)
    {
        return;
    }

    if (bConsumeOnce && bHasFired)
    {
        return;
    }

    bHasFired = true;
    OnBallHoled.Broadcast(OtherActor);
}
