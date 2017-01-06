/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#include "JUCEUnrealBridgePCH.h"
#include "AudioSourceCharacter.h"

AAudioSourceCharacter::AAudioSourceCharacter()
{
    AudioSourceComponent = CreateDefaultSubobject<UAudioSourceComponent> (TEXT ("AudioSource"));
}

void AAudioSourceCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    AudioSourceComponent->AssignPrepareToPlayCallback ([this](int samplesPerBlockExpected, double sampleRate)
    {
        PrepareToPlay (samplesPerBlockExpected, sampleRate);
    });

    AudioSourceComponent->AssignGetNextAudioBlockCallback ([this] (const juce::AudioSourceChannelInfo& bufferToFill) 
    { 
        GetNextBuffer (bufferToFill); 
    });
    
    AudioSourceComponent->RegisterAudioSource();
    AddOwnedComponent (AudioSourceComponent);
}

double AAudioSourceCharacter::GetSampleRate()
{
    if (AudioSourceComponent != nullptr)
        return AudioSourceComponent->GetSampleRate();
    return 0.0;
}

void AAudioSourceCharacter::GetNextBuffer (const juce::AudioSourceChannelInfo& bufferToFill)
{}

void AAudioSourceCharacter::PrepareToPlay (int samplesPerBlockExpected, double sampleRate)
{}