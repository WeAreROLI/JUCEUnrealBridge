/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#include "JUCEUnrealBridgePCH.h"
#include "MetronomeComponent.h"

void UMetronomeComponent::Listener::SixteenthTicked (int index) 
{ 
    NextSixteenthIndex.set (index);
    SixteenthCallback (index);
}

void UMetronomeComponent::Listener::EighthTicked (int index)    
{ 
    NextEighthIndex.set (index); 
    EighthCallback (index);
}

void UMetronomeComponent::Listener::BeatTicked (int index)      
{ 
    NextBeatIndex.set (index);
    BeatCallback (index);
}

void UMetronomeComponent::Listener::BarTicked (int index)       
{ 
    NextBarIndex.set (index);
    BarCallback (index);
}

void UMetronomeComponent::Listener::ScheduleAsyncSixteenthCallbackIfNeeded()
{
    int sixteenthIndex = NextSixteenthIndex.get();
    if (sixteenthIndex != -1)
    {
        PendingCallback.set (1);
        AsyncTask (ENamedThreads::NormalTaskPriority, [this, sixteenthIndex] () 
        {
            AsyncSixteenthCallback (sixteenthIndex);
            PendingCallback.set (0);
        });
        NextSixteenthIndex.set (-1);
    }
}

void UMetronomeComponent::Listener::ScheduleAsyncEighthCallbackIfNeeded()
{
    int eighthIndex = NextEighthIndex.get();
    if (eighthIndex != -1)
    {
        PendingCallback.set (1);
        AsyncTask (ENamedThreads::NormalTaskPriority, [this, eighthIndex] () 
        {
            AsyncEighthCallback (eighthIndex);
            PendingCallback.set (0);
        });
        NextEighthIndex.set (-1);
    }
}
void UMetronomeComponent::Listener::ScheduleAsyncBeatCallbackIfNeeded()
{
    int beatIndex = NextBeatIndex.get();
    if (beatIndex != -1)
    {
        PendingCallback.set (1);
        AsyncTask (ENamedThreads::NormalTaskPriority, [this, beatIndex] () 
        {
            AsyncBeatCallback (beatIndex);
            PendingCallback.set (0);
        });
        NextBeatIndex.set (-1);
    }
}

void UMetronomeComponent::Listener::ScheduleAsyncBarCallbackIfNeeded()
{
    int barIndex = NextBarIndex.get();
    if (barIndex != -1)
    {
        PendingCallback.set (1);
        AsyncTask (ENamedThreads::NormalTaskPriority, [this, barIndex] () 
        {
            AsyncBarCallback (barIndex);
            PendingCallback.set (0);
        });
        NextBarIndex.set (-1);
    }
}

void UMetronomeComponent::Listener::Tick()
{
    ScheduleAsyncSixteenthCallbackIfNeeded();
    ScheduleAsyncEighthCallbackIfNeeded();
    ScheduleAsyncBeatCallbackIfNeeded();
    ScheduleAsyncBarCallbackIfNeeded();
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
