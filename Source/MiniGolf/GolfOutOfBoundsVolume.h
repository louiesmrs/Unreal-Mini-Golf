#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "GolfOutOfBoundsVolume.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBallOutOfBounds, AActor *, BallActor);

UCLASS()
class MINIGOLF_API AGolfOutOfBoundsVolume : public ATriggerBox
{
    GENERATED_BODY()

public:
    AGolfOutOfBoundsVolume();

    UPROPERTY(BlueprintAssignable, Category = "MiniGolf|Events")
    FOnBallOutOfBounds OnBallOutOfBounds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGolf|Tuning")
    bool bAutoResetIfGolfBall = true;

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void HandleActorBeginOverlap(AActor *OverlappedActor, AActor *OtherActor);
};
