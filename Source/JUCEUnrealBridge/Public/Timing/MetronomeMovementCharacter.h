/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#pragma once

#include "JUCEUnrealBridgePCH.h"
#include "MetronomeListenerComponent.h"
#include "MetronomeMovementCharacter.generated.h"

/** A special character class that can synchronise its movement to a metronome.
    These characters should first register with a metronome component (using the
    RegisterWithMetronome function). Their movement velocity can then be synchronised
    with the metronome using, for example, SetJumpTimeInRhythmicUnits.

    Movements can also be scheduled according to metronomic note lengths, using 
    BeginRhythmicMove. 
*/
UCLASS()
class JUCEUNREALBRIDGE_API AMetronomeMovementCharacter : public ACharacter
{
    GENERATED_BODY()

public:
	AMetronomeMovementCharacter();
    void BeginPlay() override;

    FORCEINLINE 
    virtual void Tick (float DeltaSeconds) override
    {
        Super::Tick (DeltaSeconds);
        if (ShouldMoveTowardsTarget && CurrentRhythmicMovementLengthSeconds != 0.0f)
        {
            if (ActorHasReachedTarget())
            {
                if (CorrectMovementError)
                    SetActorLocation (RhythmicMoveTargetPosition, true);
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
                    SetActorLocation (FVector (newActorPosition.X, newActorPosition.Y, GetActorLocation().Z), true);
                else
                    SetActorLocation (newActorPosition, true);
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
        if (MetronomeListenerComponent != nullptr)
		    MetronomeListenerComponent->RegisterWithMetronome (metronome);
        UpdateJumpTime();
	}

    /** Causes the character to move continuously in the given direction (normalised)
        at a rate of 'unitsPer16th' Unreal units every metronomic 16th note. 
    */
	UFUNCTION(BlueprintCallable, Category="JUCE-MetronomeMovement")
	void SetMetronomicVelocityInUnitsPer16thNote (FVector directionVector, float unitsPer16th)
	{
        directionVector.Normalize();
		MetronomicVelocity = directionVector * unitsPer16th;
	}

	UFUNCTION(BlueprintCallable, Category="JUCE-MetronomeMovement")
	FVector GetMetronomicVeloctyInUnitsPer16thNote()
	{
		return MetronomicVelocity;
	}
    
    /** Sets the characters jump z velocity such that its jump will last
        a given number (numRhythmicUnits) of specific rhythmic subdivisions
        (rhythmicUnit). 

        For example if you want the character's jump to last 1 beat you can 
        call SetJumpTimeInRhythmicUnits (RhythmicUnitType::Beat, 1);
    */
	UFUNCTION(BlueprintCallable, Category="JUCE-MetronomeMovement")
	void SetJumpTimeInRhythmicUnits (RhythmicUnitType rhythmicUnit, int numRhythmicUnits)
	{
        JumpTimeRhythmicUnits = rhythmicUnit;
        JumpTimeNumberUnits   = numRhythmicUnits;
        UpdateJumpTime();
	}

    /** This is called during the Tick function in order to move the character
        continuously according to its metronomic velocity.
    */
	UFUNCTION(BlueprintCallable, Category="JUCE-MetronomeMovement")
	void MetronomicMove (float DeltaSeconds)
	{
        float velocityMag = 1.0f / (Get16thMovementTimeSeconds());
		SetActorLocation (GetActorLocation() + MetronomicVelocity * velocityMag * DeltaSeconds, true);
	}

    /** Causes the character to move to a given target location in the time taken 
        by a given number of rhythmic subdivisions.
    */
    UFUNCTION(BlueprintCallable, Category="JUCE-MetronomeMovement")
    void BeginRhythmicMoveToTarget (FVector targetPosition, RhythmicUnitType rhythmicUnit, int numRhythmicUnits)
    {
        RhythmicMoveStartPosition = GetActorLocation();
        
        CurrentRhythmicMovementLengthSeconds = GetRhythmicUnitMovementTimeSeconds (rhythmicUnit) * numRhythmicUnits;
        
        RhythmicMoveTargetPosition = targetPosition;
        ShouldMoveTowardsTarget = true;

        CurrentRhythmicMoveInterpTimeSeconds = 0.0f;
    }
    
    /** Causes the character to move in a given direction, at a certain speed in Unreal units per 16th, 
        for a number of rhythmic subdivisions.
    */
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

    /** Will return nullptr if the character has not been registered with any metronome component.
    */
    UFUNCTION(BlueprintCallable, Category="JUCE-MetronomeMovement")
    UMetronomeComponent* GetRegisteredMetronomeComponent()
    {
        if (MetronomeListenerComponent != nullptr)
            return MetronomeListenerComponent->GetMetronomeComponent();
        return nullptr;
    }

    /**
    This defines the proportion of time a movement lasts, in comparison to a metronomic rhythmic unit.
    When this is 1.0, there is a chance jumps will not complete before the beginning of the next
    rhythmic unit, leading to inconsistency during multiple sequential jumps.
    */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="JUCE-MetronomeMovement")
    float RhythmicUnitMovementProportion  = 0.9f;

    /**
    Setting this to false will make the BeginRhythmicMove and BeginRythmicMoveToTarget 
    functions produce completely linear movement (rather than movements that pulse in rhythm).
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
	FVector MetronomicVelocity         = FVector (0.0f, 0.0f, 0.0f);
    FVector RhythmicMoveStartPosition  = FVector (0.0f, 0.0f, 0.0f);
    FVector RhythmicMoveTargetPosition = FVector (0.0f, 0.0f, 0.0f);

    RhythmicUnitType JumpTimeRhythmicUnits = RhythmicUnitType::Beat;
    int              JumpTimeNumberUnits   = 1;

    float CurrentRhythmicMoveInterpTimeSeconds = 0.0f;
    float CurrentRhythmicMovementLengthSeconds = 0.0f;
    
    bool ShouldMoveTowardsTarget = false;

    bool ActorHasReachedTarget()
    {
        return (RhythmicMoveTargetPosition - GetActorLocation()).Size() < TargetReachedThreshold;
    }

    float Get16thMovementTimeSeconds()
    {
        if (GetRegisteredMetronomeComponent() != nullptr)
            return GetRegisteredMetronomeComponent()->GetSecondsPerSixteenth() * RhythmicUnitMovementProportion;
        return 1.0f;
    }

    float GetRhythmicUnitMovementTimeSeconds (RhythmicUnitType type)
    {
        if (GetRegisteredMetronomeComponent() != nullptr)
            return GetRegisteredMetronomeComponent()->GetSecondsPerRhythmicUnit (type) * RhythmicUnitMovementProportion;
        return 1.0f;
    }

    void UpdateJumpTime()
    {
        float jumpTime = GetRhythmicUnitMovementTimeSeconds (JumpTimeRhythmicUnits) * JumpTimeNumberUnits;
        GetCharacterMovement()->JumpZVelocity = -GetCharacterMovement()->GetGravityZ() * jumpTime * 0.5f;
    }
};