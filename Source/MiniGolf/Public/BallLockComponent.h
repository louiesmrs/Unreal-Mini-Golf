// BallLockComponent.h (Public)
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BallLockComponent.generated.h"

class AGolfBallActor;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINIGOLF_API UBallLockComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBallLockComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

    /** Teleport owner to suggested ball transform and lock movement to Ball for Radius */
    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Lock")
    void TeleportAndLockToBall(AGolfBallActor *Ball, float Radius = 350.f, float Height = 80.f, float Smooth = 8.f);

    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Lock")
    void LockToBall(AGolfBallActor *Ball, float Radius = 350.f, float Smooth = 8.f);

    UFUNCTION(BlueprintCallable, Category = "MiniGolf|Lock")
    void Unlock();

    UFUNCTION(BlueprintPure, Category = "MiniGolf|Lock")
    bool IsLocked() const { return bLocked; }

    UFUNCTION(BlueprintPure, Category = "MiniGolf|Lock")
    FVector GetLockCenter() const { return LockCenter; }

    UFUNCTION(BlueprintPure, Category = "MiniGolf|Lock")
    float GetLockRadius() const { return LockRadius; }

    /** Returns a world-space movement vector based on locked input (Forward, Right). This is suitable for AddMovementInput(GetSafeNormal(), Size()). */
    UFUNCTION(BlueprintPure, Category = "MiniGolf|Lock")
    FVector GetLockedMovementVector(float ForwardValue, float RightValue) const;

    /** Delegate broadcast when lock is acquired */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLockedSignature, AGolfBallActor *, Ball);
    /** Delegate broadcast when lock is released */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnlockedSignature, AGolfBallActor *, Ball);

    UPROPERTY(BlueprintAssignable, Category = "MiniGolf|Lock")
    FOnLockedSignature OnLocked;

    UPROPERTY(BlueprintAssignable, Category = "MiniGolf|Lock")
    FOnUnlockedSignature OnUnlocked;

protected:
    UPROPERTY(EditAnywhere, Category = "MiniGolf|Lock")
    bool bLocked = false;

    UPROPERTY(EditAnywhere, Category = "MiniGolf|Lock")
    FVector LockCenter = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, Category = "MiniGolf|Lock")
    float LockRadius = 350.f;

    UPROPERTY(EditAnywhere, Category = "MiniGolf|Lock")
    float LockSmooth = 8.f;

    UPROPERTY(EditAnywhere, Category = "MiniGolf|Lock")
    float LockedMoveSpeed = 600.f;

    TWeakObjectPtr<AGolfBallActor> LockedBall;
};
