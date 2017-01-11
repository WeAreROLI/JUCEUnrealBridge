/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#include "JUCEUnrealBridgePCH.h"
#include "MetronomeComponent.h"
void UMetronomeComponent::BeginPlay()
{
    UpdateSubdivisions();
	Sixteenth.TickCallback = [this](int index){ Listeners.call (&Listener::SixteenthTicked, index); };
	Eighth.TickCallback    = [this](int index){ Listeners.call (&Listener::EighthTicked,    index); };
	Beat.TickCallback      = [this](int index){ Listeners.call (&Listener::BeatTicked,      index); };
	Bar.TickCallback       = [this](int index){ Listeners.call (&Listener::BarTicked,       index); };

    SetTickCallback ([this]()
    {
        TickMetronome();
    });

    StartAudio();
}
