#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GolfGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGolfRunReset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGolfScoreChanged, int32, CurrentHoleStrokes, int32, TotalStrokes);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGolfHoleChanged, int32, NewHoleIndex);

UCLASS(BlueprintType)
class MINIGOLF_API UGolfGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UGolfGameInstance();

    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Run")
    void ResetRun();

    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Score")
    void AddStroke();

    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Score")
    void AddStrokes(int32 Delta);

    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Score")
    void ResetHoleStrokes();

    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Holes")
    void SetHoleIndex(int32 NewHoleIndex);

    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Holes")
    void AdvanceToNextHole();

    UFUNCTION(BlueprintPure, Category = "MiniGolf|Holes")
    int32 GetHoleIndex() const { return HoleIndex; }

    UFUNCTION(BlueprintPure, Category = "MiniGolf|Score")
    int32 GetCurrentHoleStrokes() const { return CurrentHoleStrokes; }

    UFUNCTION(BlueprintPure, Category = "MiniGolf|Score")
    int32 GetTotalStrokes() const { return TotalStrokes; }

    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Holes")
    void SetHoleMapNames(const TArray<FName> &InMapNames);

    UFUNCTION(BlueprintPure, Category = "MiniGolf|Holes")
    bool TryGetCurrentHoleMap(FName &OutMapName) const;

    UFUNCTION(BlueprintPure, Category = "MiniGolf|Holes")
    bool TryGetNextHoleMap(FName &OutMapName) const;

public:
    UPROPERTY(BlueprintAssignable, Category = "MiniGolf|Events")
    FOnGolfRunReset OnRunReset;

    UPROPERTY(BlueprintAssignable, Category = "MiniGolf|Events")
    FOnGolfScoreChanged OnScoreChanged;

    UPROPERTY(BlueprintAssignable, Category = "MiniGolf|Events")
    FOnGolfHoleChanged OnHoleChanged;

private:
    UPROPERTY(VisibleAnywhere, Category = "MiniGolf|State")
    int32 HoleIndex = 0;

    UPROPERTY(VisibleAnywhere, Category = "MiniGolf|State")
    int32 CurrentHoleStrokes = 0;

    UPROPERTY(VisibleAnywhere, Category = "MiniGolf|State")
    int32 TotalStrokes = 0;

    UPROPERTY(EditAnywhere, Category = "MiniGolf|Config")
    TArray<FName> HoleMapNames;
};
