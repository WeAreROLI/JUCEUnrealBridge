/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#include "JUCEUnrealBridgePCH.h"
#include "AudioCallbackCharacter.h"

AAudioCallbackCharacter::AAudioCallbackCharacter()
{
    AudioCallbackComponent = CreateDefaultSubobject<UAudioCallbackComponent> (TEXT ("AudioCallback"));
}

void AAudioCallbackCharacter::BeginPlay()
{
    Super::BeginPlay();
    AudioCallbackComponent->AssignAudioCallback ([this] (const float **inputChannelData, int numInputChannels, 
	    						                         float **outputChannelData, int numOutputChannels, int numSamples) 
    { 
        AudioCallback (inputChannelData, numInputChannels, outputChannelData, numOutputChannels, numSamples); 
    });
    AddOwnedComponent (AudioCallbackComponent);
    AudioCallbackComponent->StartAudio();
}

void AAudioCallbackCharacter::AudioCallback (const float **inputChannelData, int numInputChannels, 
	    						             float **outputChannelData, int numOutputChannels, int numSamples)
{}

