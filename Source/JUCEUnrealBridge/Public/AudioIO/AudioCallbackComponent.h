/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#pragma once

#include "JUCEUnrealBridgePCH.h"
#include "AudioCallbackComponent.generated.h"

UCLASS()
class UAudioCallbackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    void OnComponentDestroyed (bool bDestroyingHierarchy) override;

private:
    /*
    Since Unreal doesn't support multiple inheritance, we define this UAudioDeviceCallback class that inherits from the juce::AudioIODeviceCallback class
    Our custom Unreal Component class then owns an instance of this audio callback class, and inherits from the Unreal UActorComponent.
    */
    class UAudioDeviceCallback : public juce::AudioIODeviceCallback
    {
    public:
	    void audioDeviceIOCallback (const float **inputChannelData, int numInputChannels, 
	    						    float **outputChannelData, int numOutputChannels, int numSamples) override;
	    void audioDeviceAboutToStart (juce::AudioIODevice *device) override;
	    void audioDeviceStopped() override;

        void SetAudioDeviceIOCallback (std::function<void (const float **inputChannelData, int numInputChannels, 
	    						                           float **outputChannelData, int numOutputChannels, int numSamples)> func);

    private:
        std::function<void (const float **inputChannelData, int numInputChannels, 
	    				    float **outputChannelData, int numOutputChannels, int numSamples)> UnrealAudioDeviceIOCallback;
    };

public: 
    /*
    This function should be called by the owning object during initialisation, for example in the BeginPlay() function.
    */
    UFUNCTION(BlueprintCallable, Category = "JUCE-AudioCallback")
    void RegisterAudioCallback();

    void AssignAudioCallback (std::function<void (const float **inputChannelData, int numInputChannels, 
	    						                  float **outputChannelData, int numOutputChannels, int numSamples)> func);
    void UnregisterAudioCallback();

    double GetSampleRate();
private:
    UAudioDeviceCallback deviceCallback;
};