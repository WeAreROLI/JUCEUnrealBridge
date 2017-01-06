/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#pragma once

#include "JUCEUnrealBridgePCH.h"
#include "AudioCallbackComponent.h"
#include "AudioCallbackActor.generated.h"

UCLASS()
class JUCEUNREALBRIDGE_API AAudioCallbackActor : public AActor
{
    GENERATED_BODY()

public:
    AAudioCallbackActor();
    void BeginPlay() override;

    UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "JUCE-AudioCallback")
    UAudioCallbackComponent* AudioCallbackComponent;

    virtual void AudioCallback (const float **inputChannelData, int numInputChannels, 
	    						float **outputChannelData, int numOutputChannels, int numSamples);
private:
};