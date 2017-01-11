/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#pragma once

#include "JUCEUnrealBridgePCH.h"
#include "MetronomeListenerComponent.h"
#include "MetronomeMovementCharacter.generated.h"

UCLASS()
class JUCEUNREALBRIDGE_API AMetronomeMovementCharacter : public ACharacter
{
    GENERATED_BODY()

public:
	AMetronomeMovementCharacter();
    void BeginPlay() override;
    void EndPlay (const EEndPlayReason::Type endPlayReason) override;

    FORCEINLINE 
    virtual void Tick (float DeltaSeconds) override
    {
        Super::Tick (DeltaSeconds);
        if (ShouldMoveTowardsTarget && CurrentRhythmicMovementLengthSeconds != 0.0f)
        {
            if (ActorHasReachedTarget())
            {
                if (CorrectMovementError)
                    SetActorLocation (RhythmicMoveTargetPosition);
                ShouldMoveTowardsTarget              = false;
                CurrentRhythmicMoveInterpTimeSeconds = 0.0f;
                CurrentRhythmicMovementLengthSeconds = 0.0f;
            }
            else
            {
                CurrentRhythmicMoveInterpTimeSeconds += DeltaSeconds;
                float t = CurrentRhythmicMoveInterpTimeSeconds / CurrentRhythmicMovementLengthSeconds;
                if (UsePulseMovement)
                    t = FMath::LogX (10.0f, t * 9.0f + 1.0f);
                FVector newActorPosition = FMath::Lerp (RhythmicMoveStartPosition, RhythmicMoveTargetPosition, t);
                if (RhythmicMovementShouldIgnoreZ)
                    SetActorLocation (FVector (newActorPosition.X, newActorPosition.Y, GetActorLocation().Z));
                else
                    SetActorLocation (newActorPosition);
            }
        }
        else
        {
            MetronomicMove (DeltaSeconds);
        }
    }

    UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "JUCE-MetronomeMovement")
    UMetronomeListenerComponent* MetronomeListenerComponent;

    UFUNCTION(BlueprintCallable, Category="JUCE-MetronomeMovement")
	void RegisterWithMetronome (UMetronomeComponent* metronome)
	{
        MetronomeComponent = metronome;
        if (MetronomeListenerComponent != nullptr)
		    MetronomeListenerComponent->RegisterWithMetronome (metronome);
	}

	UFUNCTION(BlueprintCallable, Category="JUCE-MetronomeMovement")
	void DetachFromMetronome (UMetronomeComponent* metronome)
	{
		if (MetronomeListenerComponent != nullptr)
		    MetronomeListenerComponent->DetachFromMetronome (metronome);
        MetronomeComponent = nullptr;
	}

	UFUNCTION(BlueprintCallable, Category="JUCE-MetronomeMovement")
	void SetVelocityInUnitsPer16thNote (FVector directionVector, float unitsPer16th)
	{
        directionVector.Normalize();
		MetronomicVelocity = directionVector * unitsPer16th;
	}

	UFUNCTION(BlueprintCallable, Category="JUCE-MetronomeMovement")
	FVector GetVeloctyInUnitsPer16thNote()
	{
		return MetronomicVelocity;
	}

	UFUNCTION(BlueprintCallable, Category="JUCE-MetronomeMovement")
	void SetJumpTimeInRhythmicUnits (RhythmicUnitType rhythmicUnit, int numRhythmicUnits)
	{
        float jumpTime = GetRhythmicUnitMovementTimeSeconds (rhythmicUnit) * numRhythmicUnits;
        GetCharacterMovement()->JumpZVelocity = -GetCharacterMovement()->GetGravityZ() * jumpTime * 0.5f;
	}

	UFUNCTION(BlueprintCallable, Category="JUCE-MetronomeMovement")
	void MetronomicMove (float DeltaSeconds)
	{
        float velocityMag = 1.0f / (Get16thMovementTimeSeconds());
		SetActorLocation (GetActorLocation() + MetronomicVelocity * velocityMag * DeltaSeconds);
	}

    UFUNCTION(BlueprintCallable, Category="JUCE-MetronomeMovement")
    void BeginRhythmicMove (FVector directionVector, float unrealUnitsPer16th, RhythmicUnitType rhythmicUnit, int numRhythmicUnits)
    {
        directionVector.Normalize();
        FVector movementVelocity = directionVector * unrealUnitsPer16th;

        RhythmicMoveStartPosition = GetActorLocation();
        
        CurrentRhythmicMovementLengthSeconds = GetRhythmicUnitMovementTimeSeconds (rhythmicUnit) * numRhythmicUnits;
        const float num16thsInMovement       = CurrentRhythmicMovementLengthSeconds / Get16thMovementTimeSeconds();
        
        RhythmicMoveTargetPosition = RhythmicMoveStartPosition + (movementVelocity * num16thsInMovement);
        ShouldMoveTowardsTarget = true;

        CurrentRhythmicMoveInterpTimeSeconds = 0.0f;
    }
    /**
    This defines the proportion of time a movement lasts, in comparison to a metronomic rhythmic unit.
    When this is 1.0, there is a chance jumps will not complete before the beginning of the next
    rhythmic unit, leading to inconsistency during multiple sequential jumps.
    */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="JUCE-MetronomeMovement")
    float RhythmicUnitMovementProportion  = 0.9f;

    /**
    Setting this to false will make the BeginRhythmicMove function produce completely
    linear movement (rather than movements that pulse in rhythm).
    */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="JUCE-MetronomeMovement")
    bool UsePulseMovement = true;

    /**
    When the distance between the actor and the current move target is below this threshold, 
    the actor has reached its target.
    */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="JUCE-MetronomeMovement")
    float TargetReachedThreshold = 1.0f;

    /**
    When this flag is true, the actor may teleport very small unnoticable amounts after a rhythmic move, 
    if it is not exactly on target.
    */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="JUCE-MetronomeMovement")
    bool CorrectMovementError = true;

    /**
    When this is true, rhythmic movements from BeginRhythmicMove only take the X-Y plane into account.
    This ensures compatibility with the standard Character jumping implementation.
    This flag can be set to false for full 3D rhythmic movements, but this will stop the Jump commands from working.
    */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="JUCE-MetronomeMovement")
    bool RhythmicMovementShouldIgnoreZ = true;

private:
    UPROPERTY(Transient)
    UMetronomeComponent* MetronomeComponent;
	FVector MetronomicVelocity         = FVector (0.0f, 0.0f, 0.0f);
    FVector RhythmicMoveStartPosition  = FVector (0.0f, 0.0f, 0.0f);
    FVector RhythmicMoveTargetPosition = FVector (0.0f, 0.0f, 0.0f);

    float   CurrentRhythmicMoveInterpTimeSeconds = 0.0f;
    float   CurrentRhythmicMovementLengthSeconds = 0.0f;
    
    bool ShouldMoveTowardsTarget       = false;

    bool ActorHasReachedTarget()
    {
        return (RhythmicMoveTargetPosition - GetActorLocation()).Size() < TargetReachedThreshold;
    }

    float Get16thMovementTimeSeconds()
    {
        if (MetronomeComponent != nullptr)
            return MetronomeComponent->GetSecondsPerSixteenth() * RhythmicUnitMovementProportion;
        return 1.0f;
    }

    float GetRhythmicUnitMovementTimeSeconds (RhythmicUnitType type)
    {
        if (MetronomeComponent != nullptr)
            return MetronomeComponent->GetSecondsPerRhythmicUnit (type) * RhythmicUnitMovementProportion;
        return 1.0f;
    }
};