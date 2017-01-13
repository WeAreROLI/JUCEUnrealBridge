/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#include "JUCEUnrealBridgePCH.h"
#include "MetronomeComponent.h"

void UMetronomeComponent::Listener::SixteenthTicked (int index)
{
    PendingCallback.set (1);
    AsyncTask (ENamedThreads::NormalTaskPriority, [this, index] () 
    {
        SixteenthCallback (index);
        PendingCallback.set (0);
    });
}

void UMetronomeComponent::Listener::EighthTicked (int index)
{
    PendingCallback.set (1);
    AsyncTask (ENamedThreads::NormalTaskPriority, [this, index] () 
    {
        EighthCallback (index);
        PendingCallback.set (0);
    });
}

void UMetronomeComponent::Listener::BeatTicked (int index)
{
    PendingCallback.set (1);
    AsyncTask (ENamedThreads::NormalTaskPriority, [this, index] () 
    {
        BeatCallback (index);
        PendingCallback.set (0);
    });
}

void UMetronomeComponent::Listener::BarTicked (int index)
{
    PendingCallback.set (1);
    AsyncTask (ENamedThreads::NormalTaskPriority, [this, index] () 
    {
        BarCallback (index);
        PendingCallback.set (0);
    });
}

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
