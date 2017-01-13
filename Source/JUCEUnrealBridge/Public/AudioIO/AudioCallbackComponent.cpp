/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#include "JUCEUnrealBridgePCH.h"
#include "AudioCallbackComponent.h"

void UAudioCallbackComponent::UAudioDeviceCallback::setAudioDeviceAboutToStartCallback (std::function<void (juce::AudioIODevice *device)> func)
{
    unrealAudioDeviceAboutToStartCallback = func;
}

void UAudioCallbackComponent::UAudioDeviceCallback::setAudioDeviceStoppedCallback (std::function<void()> func)
{
    unrealAudioDeviceStoppedCallback = func;
}

void UAudioCallbackComponent::UAudioDeviceCallback::setAudioDeviceIOCallback (std::function<void (const float **inputChannelData, int numInputChannels, 
	    						                                              float **outputChannelData, int numOutputChannels, int numSamples)> func)
{
    unrealAudioDeviceIOCallback = func;
}

void UAudioCallbackComponent::UAudioDeviceCallback::audioDeviceIOCallback (const float **inputChannelData, int numInputChannels, 
	    						                                           float **outputChannelData, int numOutputChannels, int numSamples)
{
    if (unrealAudioDeviceIOCallback != nullptr)
        unrealAudioDeviceIOCallback (inputChannelData, numInputChannels, outputChannelData, numOutputChannels, numSamples);
}

void UAudioCallbackComponent::UAudioDeviceCallback::audioDeviceAboutToStart (juce::AudioIODevice *device) 
{
    if (unrealAudioDeviceAboutToStartCallback != nullptr)
        unrealAudioDeviceAboutToStartCallback (device);
}

void UAudioCallbackComponent::UAudioDeviceCallback::audioDeviceStopped() 
{
    if (unrealAudioDeviceStoppedCallback != nullptr)
        unrealAudioDeviceStoppedCallback();
}

//======================================================================================================
//======================================================================================================

UAudioCallbackComponent::UAudioCallbackComponent()
{
    bWantsInitializeComponent = true;
}

void UAudioCallbackComponent::InitializeComponent()
{
    deviceCallback.setAudioDeviceIOCallback ([this] (const float **inputChannelData, int numInputChannels, 
                                                     float **outputChannelData, int numOutputChannels, int numSamples)
    {
        AudioDeviceIOCallback (inputChannelData, numInputChannels, outputChannelData, numOutputChannels, numSamples);
    });

    deviceCallback.setAudioDeviceAboutToStartCallback ([this] (juce::AudioIODevice *device)
    {
        AudioDeviceAboutToStart (device);
    });

    deviceCallback.setAudioDeviceStoppedCallback ([this] ()
    {
        AudioDeviceStopped();
    });
}

void UAudioCallbackComponent::StartAudio()
{
    RegisterAudioCallback();
}

void UAudioCallbackComponent::StopAudio()
{
    UnregisterAudioCallback();
}

void UAudioCallbackComponent::RegisterAudioCallback()
{
    if (IJUCEUnrealBridge::IsAvailable())
        IJUCEUnrealBridge::Get().deviceManager.addAudioCallback (&deviceCallback);
}

void UAudioCallbackComponent::UnregisterAudioCallback()
{
    if (IJUCEUnrealBridge::IsAvailable())
        IJUCEUnrealBridge::Get().deviceManager.removeAudioCallback (&deviceCallback);
}

void UAudioCallbackComponent::OnComponentDestroyed (bool bDestroyingHierarchy)
{
    UnregisterAudioCallback();
}

double UAudioCallbackComponent::GetSampleRate()
{
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    if (IJUCEUnrealBridge::IsAvailable())
        IJUCEUnrealBridge::Get().deviceManager.getAudioDeviceSetup (setup);
    return setup.sampleRate;
}


