#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GolfScoreComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStrokeCountChanged, int32, HoleStrokes, int32, TotalStrokes);

UCLASS(ClassGroup = (MiniGolf), meta = (BlueprintSpawnableComponent))
class MINIGOLF_API UGolfScoreComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UGolfScoreComponent();

    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Score")
    void ResetAll();

    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Score")
    void ResetHole();

    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Score")
    void AddStroke();

    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Score")
    void AddStrokes(int32 Delta);

    UFUNCTION(BlueprintPure, Category = "MiniGolf|Score")
    int32 GetHoleStrokes() const { return HoleStrokes; }

    UFUNCTION(BlueprintPure, Category = "MiniGolf|Score")
    int32 GetTotalStrokes() const { return TotalStrokes; }

public:
    UPROPERTY(BlueprintAssignable, Category = "MiniGolf|Events")
    FOnStrokeCountChanged OnStrokeCountChanged;

private:
    UPROPERTY(VisibleAnywhere, Category = "MiniGolf|State")
    int32 HoleStrokes = 0;

    UPROPERTY(VisibleAnywhere, Category = "MiniGolf|State")
    int32 TotalStrokes = 0;
};
