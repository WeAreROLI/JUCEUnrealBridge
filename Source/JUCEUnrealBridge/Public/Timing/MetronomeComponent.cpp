/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#include "JUCEUnrealBridgePCH.h"
#include "MetronomeComponent.h"

void UMetronomeComponent::Listener::SixteenthTicked (int index) 
{ 
    SixteenthCallback (index);
}

void UMetronomeComponent::Listener::EighthTicked (int index)    
{ 
    EighthCallback (index);
}

void UMetronomeComponent::Listener::BeatTicked (int index)      
{ 
    BeatCallback (index);
}

void UMetronomeComponent::Listener::BarTicked (int index)       
{ 
    BarCallback (index);
}

void UMetronomeComponent::AddListener (Listener* listener) 
{
    /** Wait for listener callbacks to complete before modifying listener list. */
    while (IsPendingCallback()){}
    Listeners.add (listener); 
}

void UMetronomeComponent::RemoveListener (Listener* listener) 
{ 
    /** Wait for listener callbacks to complete before modifying listener list. */
    while (IsPendingCallback()){}
    Listeners.remove (listener); 
}

bool UMetronomeComponent::ContainsListener (Listener* listener) 
{ 
    /** Wait for listener callbacks to complete before accessing listener list. */
    while (IsPendingCallback()){}
    return Listeners.contains (listener); 
}

void UMetronomeComponent::ScheduleAsyncSixteenthCallbackIfNeeded()
{
    int sixteenthIndex = NextSixteenthIndex.get();
    if (sixteenthIndex != -1)
    {
        SixteenthCallbackPending.set (1);
        AsyncTask (ENamedThreads::NormalTaskPriority, [this, sixteenthIndex] () 
        {
            Listeners.call (&Listener::SixteenthTicked, sixteenthIndex);
            SixteenthCallbackPending.set (0);
        });
        NextSixteenthIndex.set(-1);
    }
}

void UMetronomeComponent::ScheduleAsyncEighthCallbackIfNeeded()
{
    int eighthIndex = NextEighthIndex.get();
    if (eighthIndex != -1)
    {
        EighthCallbackPending.set (1);
        AsyncTask (ENamedThreads::NormalTaskPriority, [this, eighthIndex] () 
        {
            Listeners.call (&Listener::EighthTicked, eighthIndex);
            EighthCallbackPending.set (0);
        });
        NextEighthIndex.set(-1);
    }
}

void UMetronomeComponent::ScheduleAsyncBeatCallbackIfNeeded()
{
    int beatIndex = NextBeatIndex.get();
    if (beatIndex != -1)
    {
        BeatCallbackPending.set (1);
        AsyncTask (ENamedThreads::NormalTaskPriority, [this, beatIndex] () 
        {
            Listeners.call (&Listener::BeatTicked, beatIndex);
            BeatCallbackPending.set (0);
        });
        NextBeatIndex.set(-1);
    }
}

void UMetronomeComponent::ScheduleAsyncBarCallbackIfNeeded()
{
    int barIndex = NextBarIndex.get();
    if (barIndex != -1)
    {
        BarCallbackPending.set (1);
        AsyncTask (ENamedThreads::NormalTaskPriority, [this, barIndex] () 
        {
            Listeners.call (&Listener::BarTicked, barIndex);
            BarCallbackPending.set (0);
        });
        NextBarIndex.set(-1);
    }
}

void UMetronomeComponent::BeginPlay()
{
    UpdateSubdivisions();
    NextSixteenthIndex.set (-1);
    NextEighthIndex.set    (-1);
    NextBeatIndex.set      (-1);
    NextBarIndex.set       (-1);

    SixteenthCallbackPending.set (0);
    EighthCallbackPending.set    (0);
    BeatCallbackPending.set      (0);
    BarCallbackPending.set       (0);

	Sixteenth.TickCallback = [this](int index){ NextSixteenthIndex.set (index); };
	Eighth.TickCallback    = [this](int index){ NextEighthIndex.set    (index); };
	Beat.TickCallback      = [this](int index){ NextBeatIndex.set      (index); };
	Bar.TickCallback       = [this](int index){ NextBarIndex.set       (index); };

    SetTickCallback ([this]()
    {
        TickMetronome();
    });

    StartAudio();
}
