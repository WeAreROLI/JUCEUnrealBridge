/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#include "JUCEUnrealBridgePCH.h"
#include "TimeTickerComponent.h"

void UTimeTickerComponent::InitializeComponent()
{
    Super::InitializeComponent();
    SamplesPerTick = GetSampleRate() * TickTimeSeconds;
}

void UTimeTickerComponent::AudioDeviceIOCallback (const float **inputChannelData, int numInputChannels, 
	    						                  float **outputChannelData, int numOutputChannels, int numSamples)
{
	SampleCount += numSamples;
	if (NextTickFallsInBlock (numSamples))
		if (TickCallback != nullptr)
			TickCallback();
	while (NextTickFallsInBlock (numSamples))
		NextTickSampleCount += SamplesPerTick;
}

void UTimeTickerComponent::SetTickCallback (std::function<void()> func)
{
	TickCallback = func;
}

void UTimeTickerComponent::SetTickTime (float NewTickTimeSeconds)
{
	TickTimeSeconds = (double) NewTickTimeSeconds;
	SamplesPerTick  = GetSampleRate() * TickTimeSeconds;
}

bool UTimeTickerComponent::NextTickFallsInBlock (int numSamples)
{
	return SampleCount > NextTickSampleCount - numSamples;
}

float UTimeTickerComponent::GetTickTime() { return (float) TickTimeSeconds; } 