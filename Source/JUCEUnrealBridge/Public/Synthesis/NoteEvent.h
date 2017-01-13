/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#include "NoteEvent.generated.h"

/** This class encapsulates information about a specific MIDI note event.
    The note event can be started using the StartNote function, which also
    takes in the length of the note. The ScheduleEndNote function should
    then be called, which takes in a FTimerManager and schedules the note
    off event to trigger at the correct time in the future. FTimerManager
    callbacks have to be scheduled from the game thread, whereas the StartNote
    function can theoretically be called from any thread (typically the audio
    thread). This class is therefore designed such that StartNote can be called
    from anywhere, and ScheduleEndNote is then called from the game thread.
    This is taken care of by the UNoteEvent player class.

    UNoteEventInfo keeps a pointer to any FTimerManager that is used to
    schedule note off events. This way, if a UNoteEventInfo object is destroyed
    while it has pending note off events, the timer can be cleared first.

    This class is designed such that notes can be started from the audio thread
    and corresponding note off events can be scheduled from the game thread.

    @see UNoteEventPlayer
*/
UCLASS()
class JUCEUNREALBRIDGE_API UNoteEventInfo : public UObject
{
    GENERATED_BODY()
public:
    UNoteEventInfo(){}
    ~UNoteEventInfo() 
    {
        ClearTimer();
    }

    FORCEINLINE void StartNote (juce::MidiMessageCollector* messageCollector, int midiChannel, int midiNoteNumber, 
                                float onVelocity, float offVelocity, double noteLengthMs)
    {
        MidiCollector   = messageCollector;
        MidiChannel     = midiChannel;
        MidiNote        = midiNoteNumber;
        NoteOnVelocity  = onVelocity;
        NoteOffVelocity = offVelocity;
        NoteLengthMs    = noteLengthMs;

        juce::MidiMessage m (juce::MidiMessage::noteOn (MidiChannel, MidiNote, NoteOnVelocity));
        m.setTimeStamp (juce::Time::getMillisecondCounterHiRes() * 0.001);
        MidiCollector->addMessageToQueue (m);
        IsActive = true;
        HasNoteOffScheduled = false;
    }

    FORCEINLINE void ScheduleEndNote (FTimerManager* timerManager)
    {
        if (timerManager != nullptr)
        {
            TimerManager = timerManager;
            TimerManager->ClearTimer (NoteOffHandle);
            TimerManager->SetTimer (NoteOffHandle, this, &UNoteEventInfo::EndNote, NoteLengthMs * 0.001f, false);
            HasNoteOffScheduled = true;
        }
    }

    FORCEINLINE void ClearTimer() 
    {
        if (TimerManager != nullptr)
            TimerManager->ClearTimer (NoteOffHandle);
    }

    FTimerManager* TimerManager = nullptr;

    juce::MidiMessageCollector* MidiCollector;
    UPROPERTY()
    FTimerHandle NoteOffHandle;
    UPROPERTY()
    double NoteLengthMs        = 0.0;
    UPROPERTY()
    float  NoteOnVelocity      = 0.0f;
    UPROPERTY()
    float  NoteOffVelocity     = 0.0f;
    UPROPERTY()
    int    MidiChannel         = 0;
    UPROPERTY()
    int    MidiNote            = 0;
    UPROPERTY()
    bool   IsActive            = false;
    UPROPERTY()
    bool   HasNoteOffScheduled = false;

private:
    FORCEINLINE void EndNote()
    {
        if (MidiCollector != nullptr)
        {
            juce::MidiMessage m (juce::MidiMessage::noteOff (MidiChannel, MidiNote, NoteOffVelocity));
            m.setTimeStamp (juce::Time::getMillisecondCounterHiRes() * 0.001);
            MidiCollector->addMessageToQueue (m);
        }

        MidiChannel         = 0;
        MidiNote            = 0;
        NoteOnVelocity      = 0.0f;
        NoteOffVelocity     = 0.0f;
        NoteLengthMs        = 0.0;
        IsActive            = false;
        HasNoteOffScheduled = false;
    }
};

//=======================================================================================================
//=======================================================================================================
/** This class can be used to 'play' polyphonic note sequences programmatically.
    Notes are started using the StartNoteEvent function. 
    The ScheduleNoteEndsForActiveNotes function should be called continuously 
    (for example during an Actor's Tick function) in order to schedule note off
    events for new notes that have been started.

    This class is designed such that notes can be started from the audio thread
    and the corresponding note off events can be continuously scheduled from a
    different thread.
*/
UCLASS()
class JUCEUNREALBRIDGE_API UNoteEventPlayer : public UObject
{
    GENERATED_BODY()
public:
    UNoteEventPlayer()
    {
        ClearNoteOffTimers();
    }

    FORCEINLINE void ClearNoteOffTimers()
    {
        for (int i = 0; i < NoteEvents.Num(); ++i)
            NoteEvents[i]->ClearTimer();
    }

    FORCEINLINE void SetNumberOfNoteSlots (int num)
    {
        ClearNoteOffTimers();
        NoteEvents.Empty();
        for (int i = 0; i < num; ++i)
            NoteEvents.Add (NewObject<UNoteEventInfo> (this));
    }
    
    FORCEINLINE void StartNoteEvent (juce::MidiMessageCollector* messageCollector, int midiChannel, int midiNoteNumber, 
                                     float onVelocity, float offVelocity, double noteTimeMs)
    {
        int availableNoteIndex = FindAvailableNoteIndex();
        if (availableNoteIndex != -1)
        {
            NoteEvents[availableNoteIndex]->StartNote (messageCollector, midiChannel, midiNoteNumber,
                                                      onVelocity, offVelocity, noteTimeMs);
        }
    }

    FORCEINLINE void ScheduleNoteEndsForActiveNotes (FTimerManager* timerManager)
    {
        for (int i = 0; i < NoteEvents.Num(); ++i)
        {
            if (NoteEvents[i]->IsActive && (! NoteEvents[i]->HasNoteOffScheduled))
            {
                NoteEvents[i]->ScheduleEndNote (timerManager);
            }
        }
    }

private:
    UPROPERTY(Transient)
    TArray<UNoteEventInfo*> NoteEvents;

    FORCEINLINE int FindAvailableNoteIndex()
    {
        for (int i = 0; i < NoteEvents.Num(); ++i)
        {
            if ( ! NoteEvents[i]->IsActive)
            {
                return i;
            }
        }
        return -1;
    }
};
