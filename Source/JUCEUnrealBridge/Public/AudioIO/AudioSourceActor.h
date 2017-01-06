/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#pragma once

#include "JUCEUnrealBridgePCH.h"
#include "AudioSourceComponent.h"
#include "AudioSourceActor.generated.h"

UCLASS()
class JUCEUNREALBRIDGE_API AAudioSourceActor : public AActor
{
    GENERATED_BODY()

public:
    AAudioSourceActor();
    void BeginPlay() override;

    UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "JUCE-AudioSource")
    UAudioSourceComponent* AudioSourceComponent;

    virtual void GetNextBuffer (const juce::AudioSourceChannelInfo& bufferToFill);
private:
};