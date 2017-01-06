/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#include "JUCEUnrealBridgePCH.h"
#include "AudioSourceActor.h"

AAudioSourceActor::AAudioSourceActor()
{
    AudioSourceComponent = CreateDefaultSubobject<UAudioSourceComponent> (TEXT ("AudioSource"));
}

void AAudioSourceActor::BeginPlay()
{
    Super::BeginPlay();
    AudioSourceComponent->AssignGetNextAudioBlockCallback ([this] (const juce::AudioSourceChannelInfo& bufferToFill) 
    { 
        GetNextBuffer (bufferToFill); 
    });
    AudioSourceComponent->RegisterAudioSource();
    AddOwnedComponent (AudioSourceComponent);
}

void AAudioSourceActor::GetNextBuffer (const juce::AudioSourceChannelInfo& bufferToFill)
{}

