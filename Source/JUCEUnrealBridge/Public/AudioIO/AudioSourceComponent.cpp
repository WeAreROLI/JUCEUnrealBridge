/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#include "JUCEUnrealBridgePCH.h"
#include "AudioSourceComponent.h"

void UAudioSourceComponent::UAudioSource::releaseResources() {}
void UAudioSourceComponent::UAudioSource::prepareToPlay (int newSamplesPerBlockExpected, double newSampleRate) 
{
    samplesPerBlockExpected = newSamplesPerBlockExpected;
    sampleRate              = newSampleRate;
    if (unrealPrepareToPlayCallback != nullptr)
        unrealPrepareToPlayCallback (newSamplesPerBlockExpected, newSampleRate);
}

void UAudioSourceComponent::UAudioSource::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) 
{
    if (unrealGetNextAudioBlockCallback != nullptr)
        unrealGetNextAudioBlockCallback (bufferToFill);
}

void UAudioSourceComponent::UAudioSource::setGetNextBufferCallback (std::function<void (const juce::AudioSourceChannelInfo& bufferToFill)> func) 
{
    unrealGetNextAudioBlockCallback = func;
}

void UAudioSourceComponent::UAudioSource::setPrepareToPlayCallback (std::function<void (int samplesPerBlockExpected, double sampleRate)> func) 
{
    unrealPrepareToPlayCallback = func;
}

double UAudioSourceComponent::UAudioSource::getSampleRate()              { return sampleRate; }
int    UAudioSourceComponent::UAudioSource::getSamplesPerBlockExpected() { return samplesPerBlockExpected; }

//======================================================================================================
//======================================================================================================
void UAudioSourceComponent::Initialise()
{
    RegisterAudioSource();
}

void UAudioSourceComponent::OnComponentDestroyed (bool bDestroyingHierarchy) 
{
    UnregisterAudioSource();
}

void UAudioSourceComponent::RegisterAudioSource()
{
    if (IJUCEUnrealBridge::IsAvailable())
    {
        juce::AudioDeviceManager& deviceManager = IJUCEUnrealBridge::Get().deviceManager;

        deviceManager.addAudioCallback (&audioSourcePlayer);
        audioSourcePlayer.setSource    (&audioSource);
    }
}

void UAudioSourceComponent::UnregisterAudioSource()
{
    audioSourcePlayer.setSource (nullptr);
    if (IJUCEUnrealBridge::IsAvailable())
    {
        juce::AudioDeviceManager& deviceManager = IJUCEUnrealBridge::Get().deviceManager;
        deviceManager.removeAudioCallback (&audioSourcePlayer);
    }
}

void UAudioSourceComponent::AssignGetNextAudioBlockCallback (std::function<void (const juce::AudioSourceChannelInfo& bufferToFill)> func)
{
    audioSource.setGetNextBufferCallback (func);
}

void UAudioSourceComponent::AssignPrepareToPlayCallback (std::function<void (int samplesPerBlockExpected, double sampleRate)> func)
{
    audioSource.setPrepareToPlayCallback (func);
}

double UAudioSourceComponent::GetSampleRate()
{
    return audioSource.getSampleRate();
}
