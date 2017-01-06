/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#include "JUCEUnrealBridgePCH.h"
#include "JUCEUnrealModule.h"

DEFINE_LOG_CATEGORY (ModuleLog)

#define LOCTEXT_NAMESPACE "JUCEUnreal"

JUCEUnrealModule::JUCEUnrealModule()
{}

void JUCEUnrealModule::StartupModule()
{
    deviceManager.initialise (2, 2, nullptr, true);
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup (setup);
    juce::String log ("Audio Output Device: " + setup.outputDeviceName);
    log += juce::String (" | Audio Input Device: " + setup.inputDeviceName);
	UE_LOG (ModuleLog, Warning, TEXT ("%s"), ANSI_TO_TCHAR (log.getCharPointer().getAddress()));
}

void JUCEUnrealModule::ShutdownModule()
{
    ShutDownAudio();
	UE_LOG (ModuleLog, Warning, TEXT ("JUCEUnreal: Log Ended"));
}

IMPLEMENT_MODULE    (JUCEUnrealModule, JUCEUnrealBridge);
