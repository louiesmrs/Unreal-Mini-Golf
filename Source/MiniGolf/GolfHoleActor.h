#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GolfHoleActor.generated.h"

class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBallHoled, AActor *, BallActor);

UCLASS(BlueprintType)
class MINIGOLF_API AGolfHoleActor : public AActor
{
    GENERATED_BODY()

public:
    AGolfHoleActor();

    UFUNCTION(BlueprintPure, Category = "MiniGolf|Hole")
    USphereComponent *GetTrigger() const { return Trigger; }

public:
    UPROPERTY(BlueprintAssignable, Category = "MiniGolf|Events")
    FOnBallHoled OnBallHoled;

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void HandleBeginOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor,
                            UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

private:
    UPROPERTY(VisibleAnywhere, Category = "MiniGolf|Components")
    TObjectPtr<USceneComponent> Root;

    UPROPERTY(VisibleAnywhere, Category = "MiniGolf|Components")
    TObjectPtr<USphereComponent> Trigger;

    UPROPERTY(EditAnywhere, Category = "MiniGolf|Tuning")
    bool bConsumeOnce = true;

    bool bHasFired = false;
};
