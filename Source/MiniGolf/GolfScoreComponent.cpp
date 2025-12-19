#include "GolfScoreComponent.h"

UGolfScoreComponent::UGolfScoreComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UGolfScoreComponent::ResetAll()
{
    HoleStrokes = 0;
    TotalStrokes = 0;
    OnStrokeCountChanged.Broadcast(HoleStrokes, TotalStrokes);
}

void UGolfScoreComponent::ResetHole()
{
    HoleStrokes = 0;
    OnStrokeCountChanged.Broadcast(HoleStrokes, TotalStrokes);
}

void UGolfScoreComponent::AddStroke()
{
    AddStrokes(1);
}

void UGolfScoreComponent::AddStrokes(int32 Delta)
{
    if (Delta == 0)
    {
        return;
    }

    HoleStrokes = FMath::Max(0, HoleStrokes + Delta);
    TotalStrokes = FMath::Max(0, TotalStrokes + Delta);
    OnStrokeCountChanged.Broadcast(HoleStrokes, TotalStrokes);
}
