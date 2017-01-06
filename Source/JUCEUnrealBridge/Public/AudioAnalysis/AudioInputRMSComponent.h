/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#pragma once

#include "JUCEUnrealBridgePCH.h"
#include "AudioInputRMSComponent.generated.h"

UCLASS(meta=(BlueprintSpawnableComponent), ClassGroup="JUCE-Components")
class UAudioInputRMSComponent : public UAudioCallbackComponent
{
	GENERATED_BODY()
	
private:
	FORCEINLINE void AudioCallback (const float **inputChannelData, int numInputChannels, 
	    						    float **outputChannelData, int numOutputChannels, int numSamples)
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
	void Initialise()
	{
		AssignAudioCallback ([this] (const float **inputChannelData, int numInputChannels, 
		    						                         float **outputChannelData, int numOutputChannels, int numSamples) 
	    { 
	        AudioCallback (inputChannelData, numInputChannels, outputChannelData, numOutputChannels, numSamples); 
	    });
	    RegisterAudioCallback();
	}
	
    UFUNCTION (BlueprintCallable, Category = "JUCE-AudioListener")
    float GetRMS() { return RMS.get(); }
};