/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#include "JUCEUnrealBridgePCH.h"
#include "AudioCallbackComponent.h"

UAudioCallbackComponent::UAudioCallbackComponent()
{
    bWantsInitializeComponent = true;
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

void UAudioCallbackComponent::UAudioDeviceCallback::SetAudioDeviceIOCallback (std::function<void (const float **inputChannelData, int numInputChannels, 
	    						                                              float **outputChannelData, int numOutputChannels, int numSamples)> func)
{
    UnrealAudioDeviceIOCallback = func;
}

void UAudioCallbackComponent::UAudioDeviceCallback::audioDeviceIOCallback (const float **inputChannelData, int numInputChannels, 
	    						                                           float **outputChannelData, int numOutputChannels, int numSamples)
{
    if (UnrealAudioDeviceIOCallback != nullptr)
        UnrealAudioDeviceIOCallback (inputChannelData, numInputChannels, outputChannelData, numOutputChannels, numSamples);
}

void UAudioCallbackComponent::UAudioDeviceCallback::audioDeviceAboutToStart (juce::AudioIODevice *device) {}
void UAudioCallbackComponent::UAudioDeviceCallback::audioDeviceStopped() {}

void UAudioCallbackComponent::AssignAudioCallback (std::function<void (const float **inputChannelData, int numInputChannels, 
	    						                                       float **outputChannelData, int numOutputChannels, int numSamples)> func)
{
    deviceCallback.SetAudioDeviceIOCallback (func);
}
