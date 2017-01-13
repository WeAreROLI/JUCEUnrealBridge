/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#include "JUCEUnrealBridgePCH.h"
#include "MetronomeComponent.h"

void UMetronomeComponent::Listener::SixteenthTicked (int index) { NextSixteenthIndex.set (index); }
void UMetronomeComponent::Listener::EighthTicked (int index)    { NextEighthIndex.set (index); }
void UMetronomeComponent::Listener::BeatTicked (int index)      { NextBeatIndex.set (index); }
void UMetronomeComponent::Listener::BarTicked (int index)       { NextBarIndex.set (index); }

void UMetronomeComponent::Listener::ScheduleSixteenthCallbackIfNeeded()
{
    int sixteenthIndex = NextSixteenthIndex.get();
    if (sixteenthIndex != -1)
    {
        PendingCallback.set (1);
        AsyncTask (ENamedThreads::NormalTaskPriority, [this, sixteenthIndex] () 
        {
            SixteenthCallback (sixteenthIndex);
            PendingCallback.set (0);
        });
        NextSixteenthIndex.set (-1);
    }
}

void UMetronomeComponent::Listener::ScheduleEighthCallbackIfNeeded()
{
    int eighthIndex = NextEighthIndex.get();
    if (eighthIndex != -1)
    {
        PendingCallback.set (1);
        AsyncTask (ENamedThreads::NormalTaskPriority, [this, eighthIndex] () 
        {
            EighthCallback (eighthIndex);
            PendingCallback.set (0);
        });
        NextEighthIndex.set (-1);
    }
}
void UMetronomeComponent::Listener::ScheduleBeatCallbackIfNeeded()
{
    int beatIndex = NextBeatIndex.get();
    if (beatIndex != -1)
    {
        PendingCallback.set (1);
        AsyncTask (ENamedThreads::NormalTaskPriority, [this, beatIndex] () 
        {
            BeatCallback (beatIndex);
            PendingCallback.set (0);
        });
        NextBeatIndex.set (-1);
    }
}

void UMetronomeComponent::Listener::ScheduleBarCallbackIfNeeded()
{
    int barIndex = NextBarIndex.get();
    if (barIndex != -1)
    {
        PendingCallback.set (1);
        AsyncTask (ENamedThreads::NormalTaskPriority, [this, barIndex] () 
        {
            BarCallback (barIndex);
            PendingCallback.set (0);
        });
        NextBarIndex.set (-1);
    }
}

void UMetronomeComponent::Listener::Tick()
{
    ScheduleSixteenthCallbackIfNeeded();
    ScheduleEighthCallbackIfNeeded();
    ScheduleBeatCallbackIfNeeded();
    ScheduleBarCallbackIfNeeded();
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
