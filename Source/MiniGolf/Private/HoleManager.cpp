// HoleManager.cpp
#include "HoleManager.h"
#include "HoleDef.h"
#include "../GolfGameInstance.h"
#include "../GolfHoleActor.h"
#include "../GolfBallActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "../Public/GolfPlayerController.h"
#include "../Public/BallLockComponent.h"

AHoleManager::AHoleManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AHoleManager::BeginPlay()
{
    Super::BeginPlay();
    EnsureHolesPopulated();
    if (Holes.Num() > 0)
    {
        StartHole(0);
    }
}

void AHoleManager::EnsureHolesPopulated()
{
    if (Holes.Num() > 0)
        return;

    TArray<AActor *> Found;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHoleDef::StaticClass(), Found);
    for (AActor *A : Found)
    {
        AHoleDef *H = Cast<AHoleDef>(A);
        if (H)
        {
            if (!Holes.IsValidIndex(H->HoleIndex))
            {
                Holes.SetNum(FMath::Max(Holes.Num(), H->HoleIndex + 1));
            }
            Holes[H->HoleIndex] = H;
        }
    }
}

void AHoleManager::StartHole(int32 Index)
{
    if (!Holes.IsValidIndex(Index))
    {
        UE_LOG(LogTemp, Warning, TEXT("StartHole: invalid index %d"), Index);
        return;
    }

    // Unbind previous
    UnbindCurrentHoleTrigger();

    CurrentHoleIndex = Index;
    CurrentHoleDef = Holes[Index];

    // Sync with GameInstance
    if (UGolfGameInstance *GI = GetGameInstance<UGolfGameInstance>())
    {
        GI->SetHoleIndex(Index);
        GI->ResetHoleStrokes();
    }

    AHoleDef *Def = CurrentHoleDef.Get();
    if (!Def)
        return;

    // Teleport player pawn
    if (APlayerController *PC = GetWorld()->GetFirstPlayerController())
    {
        if (APawn *P = PC->GetPawn())
        {
            FTransform Dest = Def->PlayerStartActor ? Def->PlayerStartActor->GetActorTransform() : Def->PlayerStartTransform;
            P->SetActorLocationAndRotation(Dest.GetLocation(), Dest.Rotator(), false, nullptr, ETeleportType::TeleportPhysics);
        }
    }

    // Ball setup
    AGolfBallActor *Ball = nullptr;
    if (Def->PreplacedBall)
    {
        Ball = Def->PreplacedBall;
        Ball->ResetToTransform(Def->BallStartTransform, true);
    }
    else
    {
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        AActor *Spawn = GetWorld()->SpawnActor<AGolfBallActor>(AGolfBallActor::StaticClass(), Def->BallStartTransform, Params);
        Ball = Cast<AGolfBallActor>(Spawn);
    }

    CurrentBall = Ball;

    // Bind hole trigger
    BindCurrentHoleTrigger();

    OnHoleStarted.Broadcast(Def);

    // Optional: tell player controller to look at ball
    if (APlayerController *PC = GetWorld()->GetFirstPlayerController())
    {
        if (AGolfPlayerController *GPC = Cast<AGolfPlayerController>(PC))
        {
            GPC->StartLookAtBall(Ball);
        }
    }
}

void AHoleManager::BindCurrentHoleTrigger()
{
    AHoleDef *Def = CurrentHoleDef.Get();
    if (!Def || !Def->HoleTrigger)
        return;

    Def->HoleTrigger->OnBallHoled.AddDynamic(this, &AHoleManager::HandleHoleComplete);
}

void AHoleManager::UnbindCurrentHoleTrigger()
{
    AHoleDef *Def = CurrentHoleDef.Get();
    if (!Def || !Def->HoleTrigger)
        return;

    Def->HoleTrigger->OnBallHoled.RemoveDynamic(this, &AHoleManager::HandleHoleComplete);
}

void AHoleManager::HandleHoleComplete(AActor *BallActor)
{
    AHoleDef *Def = CurrentHoleDef.Get();
    OnHoleCompleted.Broadcast(Def);

    // Stop look-at and unlock
    if (APlayerController *PC = GetWorld()->GetFirstPlayerController())
    {
        if (AGolfPlayerController *GPC = Cast<AGolfPlayerController>(PC))
        {
            GPC->StopLookAtBall();
        }

        if (APawn *P = PC->GetPawn())
        {
            if (UActorComponent *Comp = P->GetComponentByClass(UBallLockComponent::StaticClass()))
            {
                if (UBallLockComponent *BLC = Cast<UBallLockComponent>(Comp))
                {
                    BLC->Unlock();
                }
            }
        }
    }

    // Delay then advance to next hole
    FTimerHandle Handle;
    GetWorldTimerManager().SetTimer(Handle, [this]()
                                    { AdvanceToNextHole(); }, 0.75f, false);
}

void AHoleManager::AdvanceToNextHole()
{
    const int32 Next = CurrentHoleIndex + 1;
    if (Holes.IsValidIndex(Next))
    {
        StartHole(Next);
    }
    else
    {
        // End of course: notify GameInstance
        if (UGolfGameInstance *GI = GetGameInstance<UGolfGameInstance>())
        {
            // For now just set the next hole index (which will be out-of-range) and broadcast
            GI->SetHoleIndex(Next);
        }
    }
}
