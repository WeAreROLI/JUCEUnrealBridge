/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#pragma once

#include "JUCEUnrealBridgePCH.h"
#include "TimeTickerComponent.h"
#include "MetronomeComponent.generated.h"


//================================================================================
/** Various levels of subdivision for time.
*/
UENUM (BlueprintType)
enum class RhythmicUnitType : uint8
{
    Sixteenth UMETA(DisplayName = "Sixteenth"),
    Eighth 	  UMETA(DisplayName = "Eighth"),
	Beat	  UMETA(DisplayName = "Beat")
};
//================================================================================
//================================================================================

/** A time-keeping component that can be added to an Actor.
*/
UCLASS(meta=(BlueprintSpawnableComponent), ClassGroup="JUCE-Components")
class JUCEUNREALBRIDGE_API UMetronomeComponent : public UTimeTickerComponent
{
	GENERATED_BODY()

public:
    /** UMetronomeComponent::Listener objects can register with a metronome component
        and respond to rhythmic subdivision events.

        Listener callbacks are called offloaded from the audio thread to a different thread
        in the MetronomeComponent (see below).
    */
    class Listener
    {
    public:
        friend class UMetronomeComponent;

        Listener() 
        {}

        virtual void SixteenthCallback (int index) {}
        virtual void EighthCallback    (int index) {}
        virtual void BeatCallback      (int index) {}
        virtual void BarCallback       (int index) {}

    private:
        void SixteenthTicked (int index);
        void EighthTicked    (int index);
        void BeatTicked      (int index);
        void BarTicked       (int index);
    };

    void AddListener      (Listener* listener);
    void RemoveListener   (Listener* listener);
    bool ContainsListener (Listener* listener);

private:
    juce::ListenerList<Listener> Listeners;

    /** The RhythmicUnit srtucture represents a particular rhythmic subdivision
        that can be 'ticked' over time.
    */
    struct RhythmicUnit
    {
        RhythmicUnit() 
        : CurrentIndex (-1), NumSubdivisions (4)
        {}

        RhythmicUnit (int nSubdivisions) 
        : CurrentIndex (-1), NumSubdivisions (nSubdivisions)
        {}

        void Tick()
        {
            CurrentIndex ++;
            if (TickCallback != 0)
                TickCallback (CurrentIndex);
        }

        int GetModuloTickIndex() { return CurrentIndex % NumSubdivisions; }

        std::function<void (int index)> TickCallback;
        int CurrentIndex;

        /** Indicates the number of RhythmicUnit subdivisions that make up the level above.
        */
        int NumSubdivisions;
    };

    template<typename R>
    FORCEINLINE void TickRhythmicUnit (R& unit) { unit.Tick(); }

    template<typename R, typename... Args>
    FORCEINLINE void TickRhythmicUnit (R& unit, Args&... args) 
    {
        unit.Tick();
        if (unit.GetModuloTickIndex() == 0)
            TickRhythmicUnit (args...);
    }

public:
    UMetronomeComponent()
    {
        NextSixteenthIndex.set (-1);
        NextEighthIndex.set    (-1);
        NextBeatIndex.set      (-1);
        NextBarIndex.set       (-1);

        SixteenthCallbackPending.set (0);
        EighthCallbackPending.set    (0);
        BeatCallbackPending.set      (0);
        BarCallbackPending.set       (0);
        
        PrimaryComponentTick.bCanEverTick = true;
    }

	void BeginPlay() override;

    FORCEINLINE void OnComponentDestroyed (bool bDestroyingHierarchy) override
    {
        Super::OnComponentDestroyed (bDestroyingHierarchy);
        StopMetronome();
    }

    /** We continuously schedule any required callback events
    */
    FORCEINLINE void TickComponent (float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override
    {
        ScheduleAsyncSixteenthCallbackIfNeeded();
        ScheduleAsyncEighthCallbackIfNeeded();
        ScheduleAsyncBeatCallbackIfNeeded();
        ScheduleAsyncBarCallbackIfNeeded();
    }

    UFUNCTION (BlueprintCallable, Category = "JUCE-Metronome")
    void StartMetronome()
    {
        MetronomeRunning = true;
    }

    UFUNCTION (BlueprintCallable, Category = "JUCE-Metronome")
    void StopMetronome()
    {
        MetronomeRunning = false;
        WaitForMetronomeCallbacksToComplete();
    }

    /** Wait for all callbacks to finish.
    */
    UFUNCTION (BlueprintCallable, Category = "JUCE-Metronome")
    void WaitForMetronomeCallbacksToComplete()
    {
        while (IsPendingCallback()) {}
    }

    UFUNCTION (BlueprintCallable, Category = "JUCE-Metronome")
    void SetTempo (float bpm)
    {
    	double beatsPerSecond      = bpm / 60.0;
    	double secondsPerSixteenth = 1.0 / (beatsPerSecond * 4.0f);
    	SetTickTime ((float) secondsPerSixteenth);
    } 

    UFUNCTION (BlueprintCallable, Category = "JUCE-Metronome")
    void SetTimeSignature (int numerator, RhythmicUnitType divisor) 
    {
    	Numerator = numerator;
    	Divisor   = divisor;
    	UpdateSubdivisions();
    }
    UFUNCTION (BlueprintCallable, Category = "JUCE-Metronome")
    float GetSecondsPerRhythmicUnit (RhythmicUnitType type)
    {
        switch (type)
        {
            case RhythmicUnitType::Sixteenth: return GetSecondsPerSixteenth();
            case RhythmicUnitType::Eighth:    return GetSecondsPerEighth();
            case RhythmicUnitType::Beat:      return GetSecondsPerBeat();
            default:                          return GetSecondsPerSixteenth();
        }
    }

    UFUNCTION (BlueprintCallable, Category = "JUCE-Metronome")
    float GetSecondsPerSixteenth()
    {
        return GetTickTime();
    }

    UFUNCTION (BlueprintCallable, Category = "JUCE-Metronome")
    float GetSecondsPerEighth()
    {
        return GetTickTime() * 2.0f;
    }

    UFUNCTION (BlueprintCallable, Category = "JUCE-Metronome")
    float GetSecondsPerBeat()
    {
        return GetTickTime() * 4.0f;
    }

private:
    bool MetronomeRunning = false;
	int  Numerator        = 4;
	RhythmicUnitType Divisor = RhythmicUnitType::Beat;

    juce::Atomic<int> NextSixteenthIndex;
    juce::Atomic<int> NextEighthIndex;
    juce::Atomic<int> NextBeatIndex;
    juce::Atomic<int> NextBarIndex;
    juce::Atomic<int> SixteenthCallbackPending;
    juce::Atomic<int> EighthCallbackPending;
    juce::Atomic<int> BeatCallbackPending;
    juce::Atomic<int> BarCallbackPending;
    RhythmicUnit Sixteenth;
    RhythmicUnit Eighth;
    RhythmicUnit Beat;
    RhythmicUnit Bar;

    bool IsPendingCallback()
    {
        return SixteenthCallbackPending.get() == 1 || EighthCallbackPending.get() == 1 || BeatCallbackPending.get() == 1 || BarCallbackPending.get() == 1;
    }

    void ScheduleAsyncSixteenthCallbackIfNeeded();
    void ScheduleAsyncEighthCallbackIfNeeded();
    void ScheduleAsyncBeatCallbackIfNeeded();
    void ScheduleAsyncBarCallbackIfNeeded();

    FORCEINLINE void UpdateSubdivisions()
    {
    	Sixteenth.NumSubdivisions = 2;
    	Eighth.NumSubdivisions    = 2;
    	Beat.NumSubdivisions      = 4;
    	switch (Divisor)
    	{
    		case RhythmicUnitType::Sixteenth: Sixteenth.NumSubdivisions = Numerator; break;
    		case RhythmicUnitType::Eighth:    Eighth.NumSubdivisions    = Numerator; break;
    		case RhythmicUnitType::Beat:      Beat.NumSubdivisions      = Numerator; break;
    	}
    }

    FORCEINLINE void TickMetronome()
    {
        if (MetronomeRunning)
        {
    	    switch (Divisor)
    	    {
    		    case RhythmicUnitType::Sixteenth: TickRhythmicUnit (Sixteenth, Bar);               break;
    		    case RhythmicUnitType::Eighth:    TickRhythmicUnit (Sixteenth, Eighth, Bar);       break;
    		    case RhythmicUnitType::Beat:      TickRhythmicUnit (Sixteenth, Eighth, Beat, Bar); break;
    	    }
        }
    }

    
};
