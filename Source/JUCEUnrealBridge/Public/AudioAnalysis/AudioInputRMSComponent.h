/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#pragma once

#include "JUCEUnrealBridgePCH.h"
#include "AudioInputRMSComponent.generated.h"

/*
    A Simple Component that keeps a running calculation of the RMS Amplitude of the audio input.
    Future extensions to the plugin will include full audio feature extraction capabilities.
*/

UCLASS(meta=(BlueprintSpawnableComponent), ClassGroup="JUCE-Components")
class UAudioInputRMSComponent : public UAudioCallbackComponent
{
	GENERATED_BODY()
	
private:
	FORCEINLINE virtual void AudioDeviceIOCallback (const float **inputChannelData, int numInputChannels, 
	    						                    float **outputChannelData, int numOutputChannels, int numSamples) override
	{
		const float* const* inputData = inputChannelData;
	    juce::AudioSampleBuffer buffer (numInputChannels, numSamples);
	    float rmsSum = 0.0f;
	    for (int c = 0; c < numInputChannels; ++c)
	    {
	        buffer.copyFrom (c, 0, inputData[c], numSamples);
	        rmsSum += buffer.getRMSLevel (c, 0, numSamples);
	    }
	    if (numInputChannels > 0)
	        RMS.set (rmsSum / numInputChannels);

	    // We need to clear the output buffers, in case they're full of junk..
	    for (int i = 0; i < numOutputChannels; ++i)
	        if (outputChannelData[i] != nullptr)
	            juce::FloatVectorOperations::clear (outputChannelData[i], numSamples);
	}

    juce::Atomic<float> RMS;

public:
	
    UFUNCTION (BlueprintCallable, Category = "JUCE-AudioListener")
    float GetRMS() { return RMS.get(); }
};