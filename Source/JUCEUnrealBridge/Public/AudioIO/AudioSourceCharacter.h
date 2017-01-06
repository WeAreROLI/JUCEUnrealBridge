/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#pragma once

#include "JUCEUnrealBridgePCH.h"
#include "AudioSourceComponent.h"
#include "AudioSourceCharacter.generated.h"

UCLASS()
class JUCEUNREALBRIDGE_API AAudioSourceCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AAudioSourceCharacter();
    void BeginPlay() override;
    double GetSampleRate();

    UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "JUCE-AudioSource")
    UAudioSourceComponent* AudioSourceComponent;

    virtual void GetNextBuffer (const juce::AudioSourceChannelInfo& bufferToFill);
    virtual void PrepareToPlay (int samplesPerBlockExpected, double sampleRate);
private:
};