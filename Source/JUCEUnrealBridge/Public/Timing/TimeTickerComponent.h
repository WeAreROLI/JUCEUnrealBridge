/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#pragma once

#include "JUCEUnrealBridgePCH.h"
#include "AudioCallbackComponent.h"
#include "TimeTickerComponent.generated.h"

/** A TimeTickerComponent continuously 'ticks' at a particular frequency 
    and is accurate at the audio sample level. Tick callbacks are called
    from the audio thread, and shouldn't involve any heavy processing 
    (this should be offloaded onto other threads).
    
    This class serves as a basis for extended functionality.
    @see MetronomeComponent.
*/
UCLASS()
class JUCEUNREALBRIDGE_API UTimeTickerComponent : public UAudioCallbackComponent
{
    GENERATED_BODY()

public:
    void AudioDeviceIOCallback (const float **inputChannelData, int numInputChannels, 
	    				        float **outputChannelData, int numOutputChannels, int numSamples) override;
    
    void SetTickCallback (std::function<void()> func);

    UFUNCTION (BlueprintCallable, Category = "JUCE-TimeTicker")
    void SetTickTime (float NewTickTimeSeconds);
    UFUNCTION (BlueprintCallable, Category = "JUCE-TimeTicker")
    float GetTickTime();

    void InitializeComponent() override;

private:
    double TickTimeSeconds     = 0.125;
    double SampleCount         = 0.0;
    double NextTickSampleCount = 0.0;
    double SamplesPerTick      = 0.0;

    std::function<void()> TickCallback;
    
    bool NextTickFallsInBlock (int numSamples);
};