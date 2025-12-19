#include "GolfGameInstance.h"

UGolfGameInstance::UGolfGameInstance() = default;

void UGolfGameInstance::ResetRun()
{
    HoleIndex = 0;
    CurrentHoleStrokes = 0;
    TotalStrokes = 0;
    OnRunReset.Broadcast();
    OnHoleChanged.Broadcast(HoleIndex);
    OnScoreChanged.Broadcast(CurrentHoleStrokes, TotalStrokes);
}

void UGolfGameInstance::AddStroke()
{
    AddStrokes(1);
}

void UGolfGameInstance::AddStrokes(int32 Delta)
{
    if (Delta == 0)
    {
        return;
    }

    CurrentHoleStrokes = FMath::Max(0, CurrentHoleStrokes + Delta);
    TotalStrokes = FMath::Max(0, TotalStrokes + Delta);
    OnScoreChanged.Broadcast(CurrentHoleStrokes, TotalStrokes);
}

void UGolfGameInstance::ResetHoleStrokes()
{
    CurrentHoleStrokes = 0;
    OnScoreChanged.Broadcast(CurrentHoleStrokes, TotalStrokes);
}

void UGolfGameInstance::SetHoleIndex(int32 NewHoleIndex)
{
    HoleIndex = FMath::Max(0, NewHoleIndex);
    OnHoleChanged.Broadcast(HoleIndex);
}

void UGolfGameInstance::AdvanceToNextHole()
{
    SetHoleIndex(HoleIndex + 1);
    ResetHoleStrokes();
}

void UGolfGameInstance::SetHoleMapNames(const TArray<FName> &InMapNames)
{
    HoleMapNames = InMapNames;
}

bool UGolfGameInstance::TryGetCurrentHoleMap(FName &OutMapName) const
{
    if (!HoleMapNames.IsValidIndex(HoleIndex))
    {
        return false;
    }
    OutMapName = HoleMapNames[HoleIndex];
    return true;
}

bool UGolfGameInstance::TryGetNextHoleMap(FName &OutMapName) const
{
    const int32 NextIndex = HoleIndex + 1;
    if (!HoleMapNames.IsValidIndex(NextIndex))
    {
        return false;
    }
    OutMapName = HoleMapNames[NextIndex];
    return true;
}
