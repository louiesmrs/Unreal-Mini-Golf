// HoleDef.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HoleDef.generated.h"

class AGolfHoleActor;
class AGolfBallActor;

UCLASS(Blueprintable)
class MINIGOLF_API AHoleDef : public AActor
{
    GENERATED_BODY()

public:
    AHoleDef();

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "MiniGolf|Hole")
    int32 HoleIndex = 0;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "MiniGolf|Hole")
    AActor *PlayerStartActor = nullptr;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "MiniGolf|Hole")
    FTransform PlayerStartTransform = FTransform::Identity;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "MiniGolf|Hole")
    FTransform BallStartTransform = FTransform::Identity;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "MiniGolf|Hole")
    AGolfHoleActor *HoleTrigger = nullptr;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "MiniGolf|Hole")
    AGolfBallActor *PreplacedBall = nullptr;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "MiniGolf|Hole")
    int32 Par = 3;
};
