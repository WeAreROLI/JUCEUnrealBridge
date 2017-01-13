/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#pragma once

#include "JUCEUnrealBridgePCH.h"
#include "SynthesiserComponent.h"
#include "SynthesiserActor.generated.h"

/** An actor that has a synthesiser component, as well as functions to set the parameters in the synth. 
*/
UCLASS()
class JUCEUNREALBRIDGE_API ASynthesiserActor : public AActor
{
    GENERATED_BODY()

public:
	ASynthesiserActor();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="JUCE-SynthActor")
	void SetAttackRateSeconds (float newRate);
	UFUNCTION(BlueprintCallable, Category="JUCE-SynthActor")
	void SetDecayRateSeconds (float newRate);
	UFUNCTION(BlueprintCallable, Category="JUCE-SynthActor")
	void SetSustainLevel (float newLevel);
	UFUNCTION(BlueprintCallable, Category="JUCE-SynthActor")
	void SetReleaseRateSeconds (float newRate);

	UFUNCTION(BlueprintCallable, Category="JUCE-SynthActor")
	float GetAttackRateSeconds();
	UFUNCTION(BlueprintCallable, Category="JUCE-SynthActor")
	float GetDecayRateSeconds();
	UFUNCTION(BlueprintCallable, Category="JUCE-SynthActor")
	float GetSustainLevel();
	UFUNCTION(BlueprintCallable, Category="JUCE-SynthActor")
	float GetReleaseRateSeconds();

	UPROPERTY(EditAnywhere, Category="JUCE-SynthActor")
	float InitialAttackRateSeconds = 0.01f;

	UPROPERTY(EditAnywhere, Category="JUCE-SynthActor")
	float InitialDecayRateSeconds = 0.1f;

	UPROPERTY(EditAnywhere, Category="JUCE-SynthActor")
	float InitialSustainLevel = 0.8f;

	UPROPERTY(EditAnywhere, Category="JUCE-SynthActor")
	float InitialReleaseRateSeconds = 0.1f;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "JUCE-MetronomeMovement")
    USynthesiserComponent* SynthesiserComponent;

private:
	float AttackRateSeconds  = 0.01f;
	float DecayRateSeconds   = 0.1f;
	float SustainLevel       = 0.8f;
	float ReleaseRateSeconds = 0.1f;
};