/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#include "JUCEUnrealBridgePCH.h"
#include "AudioSourceComponent.h"

void UAudioSourceComponent::UAudioSource::releaseResources() 
{
    if (unrealReleaseResourcesCallback != nullptr)
        unrealReleaseResourcesCallback();
}

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

void UAudioSourceComponent::UAudioSource::setReleaseResourcesCallback (std::function<void()> func)
{
    unrealReleaseResourcesCallback = func;
}

double UAudioSourceComponent::UAudioSource::getSampleRate()              { return sampleRate; }
int    UAudioSourceComponent::UAudioSource::getSamplesPerBlockExpected() { return samplesPerBlockExpected; }

//======================================================================================================
//======================================================================================================

UAudioSourceComponent::UAudioSourceComponent()
{
    bWantsInitializeComponent = true;
}

void UAudioSourceComponent::InitializeComponent()
{
    audioSource.setGetNextBufferCallback ([this] (const juce::AudioSourceChannelInfo& bufferToFill)
    {
        GetNextAudioBlock (bufferToFill);
    });

    audioSource.setPrepareToPlayCallback ([this] (int samplesPerBlockExpected, double sampleRate)
    {
        PrepareToPlay (samplesPerBlockExpected, sampleRate);
    });
}

void UAudioSourceComponent::StartAudio()
{
    RegisterAudioSource();
}

void UAudioSourceComponent::StopAudio()
{
    UnregisterAudioSource();
}

void UAudioSourceComponent::OnComponentDestroyed (bool bDestroyingHierarchy) 
{
    UnregisterAudioSource();
}

double UAudioSourceComponent::GetSampleRate()
{
    return audioSource.getSampleRate();
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

