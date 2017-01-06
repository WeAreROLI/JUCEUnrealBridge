/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#include "JUCEUnrealBridgePCH.h"
#include "AudioCallbackActor.h"

AAudioCallbackActor::AAudioCallbackActor()
{
    AudioCallbackComponent = CreateDefaultSubobject<UAudioCallbackComponent> (TEXT ("AudioCallback"));
}

void AAudioCallbackActor::BeginPlay()
{
    Super::BeginPlay();
    AudioCallbackComponent->AssignAudioCallback ([this] (const float **inputChannelData, int numInputChannels, 
	    						                         float **outputChannelData, int numOutputChannels, int numSamples) 
    { 
        AudioCallback (inputChannelData, numInputChannels, outputChannelData, numOutputChannels, numSamples); 
    });
    AudioCallbackComponent->RegisterAudioCallback();
    AddOwnedComponent (AudioCallbackComponent);
}

void AAudioCallbackActor::AudioCallback (const float **inputChannelData, int numInputChannels, 
	    						         float **outputChannelData, int numOutputChannels, int numSamples)
{}

