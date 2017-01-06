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
	Sixteenth.TickCallback = [this](int index){ SixteenthTickCallback (index); Listeners.call (&Listener::SixteenthTicked, index); };
	Eighth.TickCallback    = [this](int index){ EighthTickCallback (index);    Listeners.call (&Listener::EigthTicked,     index); };
	Beat.TickCallback      = [this](int index){ BeatTickCallback (index);      Listeners.call (&Listener::BeatTicked,      index); };
	Bar.TickCallback       = [this](int index){ BarTickCallback (index);       Listeners.call (&Listener::BarTicked,       index); };

    SetTickCallback ([this]()
    {
        TickMetronome();
    });
}

void ReceiveSixteenthTick_Implementation() {}
void ReceiveEigthTick_Implementation()     {}
void ReceiveBeatTick_Implementation()      {}
void ReceiveBarTick_Implementation()       {}

void UMetronomeComponent::SixteenthTickCallback (int index) { OnSixteenth.Broadcast (index); }
void UMetronomeComponent::EighthTickCallback    (int index) { OnEighth.Broadcast (index);    }
void UMetronomeComponent::BeatTickCallback      (int index) { OnBeat.Broadcast (index);      }
void UMetronomeComponent::BarTickCallback       (int index) { OnBar.Broadcast (index);       }
