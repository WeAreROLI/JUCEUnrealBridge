/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#pragma once

#define JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED true
#define DONT_SET_USING_JUCE_NAMESPACE true

// Unreal
#include "Engine.h"
#include "ModuleManager.h"
#include "Internationalization.h"
#include "SlateBasics.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"

// JUCE
#include "juce_core/juce_core.h"
#include "juce_cryptography/juce_cryptography.h"
#include "juce_events/juce_events.h"
#include "juce_data_structures/juce_data_structures.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_gui_extra/juce_gui_extra.h"
#include "juce_opengl/juce_opengl.h"
#include "juce_video/juce_video.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_audio_devices/juce_audio_devices.h"
#include "juce_audio_formats/juce_audio_formats.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_audio_utils/juce_audio_utils.h"

#include "Synthesis/UnrealSynthesiserClasses.h"
#include "IJUCEUnrealBridge.h"
