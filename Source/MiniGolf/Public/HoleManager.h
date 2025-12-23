// HoleManager.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HoleManager.generated.h"

class AHoleDef;
class AGolfBallActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHoleStarted, AHoleDef *, HoleDef);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHoleCompleted, AHoleDef *, HoleDef);

UCLASS(Blueprintable)
class MINIGOLF_API AHoleManager : public AActor
{
    GENERATED_BODY()

public:
    AHoleManager();

    virtual void BeginPlay() override;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "MiniGolf|Holes")
    TArray<AHoleDef *> Holes;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniGolf|Holes")
    int32 CurrentHoleIndex = -1;

    UPROPERTY(BlueprintAssignable, Category = "MiniGolf|Events")
    FOnHoleStarted OnHoleStarted;

    UPROPERTY(BlueprintAssignable, Category = "MiniGolf|Events")
    FOnHoleCompleted OnHoleCompleted;

    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Holes")
    void StartHole(int32 Index);

    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Holes")
    void AdvanceToNextHole();

    UFUNCTION()
    void HandleHoleComplete(AActor *BallActor);

protected:
    void EnsureHolesPopulated();
    void BindCurrentHoleTrigger();
    void UnbindCurrentHoleTrigger();

private:
    TWeakObjectPtr<AGolfBallActor> CurrentBall;
    TWeakObjectPtr<AHoleDef> CurrentHoleDef;
};
