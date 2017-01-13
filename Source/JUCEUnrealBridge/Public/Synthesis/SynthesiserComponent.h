/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#pragma once

#include "AudioIO/AudioSourceComponent.h"
#include "WaveVoice.h"
#include "SynthesiserComponent.generated.h"

UCLASS(meta=(BlueprintSpawnableComponent), ClassGroup="JUCE-Components")
class JUCEUNREALBRIDGE_API USynthesiserComponent : public UAudioSourceComponent
{
	GENERATED_BODY()

public:
	USynthesiserComponent()
    {
        bWantsInitializeComponent = true;
        Initialised = false;
        PrimaryComponentTick.bCanEverTick = true;
    }

    FORCEINLINE virtual void InitializeComponent() override
    {
        Super::InitializeComponent();

        NotePlayer = NewObject<UNoteEventPlayer> (this);
        NotePlayer->SetNumberOfNoteSlots (1);

        // Add some voices to our synth, to play the sounds..
        Synth.clearVoices();
        for (int i = 4; --i >= 0;)
        {
            Synth.addVoice (new UWaveVoice());   // These voices will play our custom wave sounds..
        }

        // ..and add a sound for them to play...
        Synth.clearSounds();
        Synth.addSound (new UWaveVoice::UWaveSound());
        
        StartAudio();

        SetWaveformType       (InitialWaveType);
        SetAttackRateSeconds  (InitialAttackRateSeconds);
        SetDecayRateSeconds   (InitialDecayRateSeconds);
        SetSustainLevel       (InitialSustainLevel);
        SetReleaseRateSeconds (InitialReleaseRateSeconds);
    }

    /** We continuously schedule any required note off events for UNoteEventInfo note events that have been started.
        @see UNoteEventInfo, UNoteEventPlayer.
    */
    FORCEINLINE virtual void TickComponent (float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override
    {
        if (Initialised)
            NotePlayer->ScheduleNoteEndsForActiveNotes (&GetWorld()->GetTimerManager());
    }

    UFUNCTION(BlueprintCallable, Category = "JUCE-Synthesiser")
    void SetWaveformType (WaveType w)
    {
        CurrentWaveType = w;
        for (int i = 0; i < Synth.getNumVoices(); ++i)
        {
            UWaveVoice* voice = (UWaveVoice*) Synth.getVoice (i);
            if (voice != nullptr)
                voice->SetWaveformType (w);
        }
    }

    UFUNCTION(BlueprintCallable, Category = "JUCE-Synthesiser")
    void SetAttackRateSeconds (float rate) 
    {
        rate = CheckRate (rate);
        CurrentAttackRateSeconds = rate;
        for (int i = 0; i < Synth.getNumVoices(); ++i)
        {
            UWaveVoice* voice = (UWaveVoice*) Synth.getVoice (i);
            if (voice != nullptr)
                voice->SetAttackRateSeconds ((double) rate);
        }
    }

    UFUNCTION(BlueprintCallable, Category = "JUCE-Synthesiser")
    void SetDecayRateSeconds (float rate) 
    {
        rate = CheckRate (rate);
        CurrentDecayRateSeconds = rate;
        for (int i = 0; i < Synth.getNumVoices(); ++i)
        {
            UWaveVoice* voice = (UWaveVoice*) Synth.getVoice (i);
            if (voice != nullptr)
                voice->SetDecayRateSeconds ((double) rate);
        }
    }

    UFUNCTION(BlueprintCallable, Category = "JUCE-Synthesiser")
    void SetReleaseRateSeconds (float rate) 
    {
        rate = CheckRate (rate);
        CurrentReleaseRateSeconds = rate;
        for (int i = 0; i < Synth.getNumVoices(); ++i)
        {
            UWaveVoice* voice = (UWaveVoice*) Synth.getVoice (i);
            if (voice != nullptr)
                voice->SetReleaseRateSeconds ((double) rate);
        }
    }

    UFUNCTION(BlueprintCallable, Category = "JUCE-Synthesiser")
    void SetSustainLevel (float level)
    {
        CurrentSustainLevel = level;
        for (int i = 0; i < Synth.getNumVoices(); ++i)
        {
            UWaveVoice* voice = (UWaveVoice*) Synth.getVoice (i);
            if (voice != nullptr)
                voice->SetSustainLevel ((double) level);
        }
    }

    UFUNCTION(BlueprintCallable, Category = "JUCE-Synthesiser")
    WaveType GetWaveformType() { return CurrentWaveType; }

    UFUNCTION(BlueprintCallable, Category = "JUCE-Synthesiser")
    float GetAttackRateSeconds() { return CurrentAttackRateSeconds; }

    UFUNCTION(BlueprintCallable, Category = "JUCE-Synthesiser")
    float GetDecayRateSeconds() { return CurrentDecayRateSeconds; }

    UFUNCTION(BlueprintCallable, Category = "JUCE-Synthesiser")
    float GetReleaseRateSeconds() { return CurrentReleaseRateSeconds; }

    UFUNCTION(BlueprintCallable, Category = "JUCE-Synthesiser")
    float GetSustainLevel() { return CurrentSustainLevel; }

    /** This function can be used to play a note of a given length (in milliseconds).
        @see UNoteEventInfo, UNoteEvenPlayer.
    */
    UFUNCTION(BlueprintCallable, Category = "JUCE-Synthesiser")
    void PlayNoteEvent (int midiChannel, int midiNoteNumber, float onVelocity, float offVelocity, float timeMs)
    {
        if (Initialised)
            NotePlayer->StartNoteEvent (&MidiCollector, midiChannel, midiNoteNumber, onVelocity, offVelocity, timeMs);
        else
            UE_LOG(LogTemp, Warning, TEXT("Attempted to play notes using an uninitialized synthesiser component. Make sure you call StartAudio() before using the synthesiser (for example during BeginPlay())"));
    }

    UFUNCTION(BlueprintCallable, Category = "JUCE-Synthesiser")
    void TriggerNoteOn (int midiChannel, int midiNoteNumber, float velocity)
    {
        if (Initialised)
        {
            juce::MidiMessage m (juce::MidiMessage::noteOn (midiChannel, midiNoteNumber, velocity));
            m.setTimeStamp (juce::Time::getMillisecondCounterHiRes() * 0.001);

            MidiCollector.addMessageToQueue (m);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Attempted to play notes using an uninitialized synthesiser component. Make sure you call StartAudio() before using the synthesiser (for example during BeginPlay())"));
        }
    }

    UFUNCTION(BlueprintCallable, Category = "JUCE-Synthesiser")
    void TriggerNoteOff (int midiChannel, int midiNoteNumber, float velocity)
    {
        if (Initialised)
        {
            juce::MidiMessage m (juce::MidiMessage::noteOff (midiChannel, midiNoteNumber, velocity));
            m.setTimeStamp (juce::Time::getMillisecondCounterHiRes() * 0.001);

            MidiCollector.addMessageToQueue (m);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Attempted to play notes using an uninitialized synthesiser component. Make sure you call StartAudio() before using the synthesiser (for example during BeginPlay())"));
        }
    }

    UPROPERTY(EditAnywhere, Category="JUCE-Synthesiser")
    WaveType InitialWaveType = WaveType::Sin;
    UPROPERTY(EditAnywhere, Category="JUCE-Synthesiser")
    float InitialAttackRateSeconds  = 0.01f;
    UPROPERTY(EditAnywhere, Category="JUCE-Synthesiser")
	float InitialDecayRateSeconds   = 0.1f;
    UPROPERTY(EditAnywhere, Category="JUCE-Synthesiser")
	float InitialSustainLevel       = 0.8f;
    UPROPERTY(EditAnywhere, Category="JUCE-Synthesiser")
	float InitialReleaseRateSeconds = 0.1f;

protected:
    FORCEINLINE virtual void PrepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        MidiCollector.reset (sampleRate);

        Synth.setCurrentPlaybackSampleRate (sampleRate);

        Initialised = true;
    }

    FORCEINLINE virtual void GetNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        // the synth always adds its output to the audio buffer, so we have to clear it
        // first..
        bufferToFill.clearActiveBufferRegion();

        // fill a midi buffer with incoming messages from the midi input.
        juce::MidiBuffer incomingMidi;
        MidiCollector.removeNextBlockOfMessages (incomingMidi, bufferToFill.numSamples);

        // and now get the synth to process the midi events and generate its output.
        Synth.renderNextBlock (*bufferToFill.buffer, incomingMidi, 0, bufferToFill.numSamples);
    }

public:
    //==============================================================================
    // this collects real-time midi messages from the midi input device, and
    // turns them into blocks that we can process in our audio callback
    juce::MidiMessageCollector MidiCollector;

    UPROPERTY(Transient)
    UNoteEventPlayer* NotePlayer;

    // the underlying juce::Synthesiser object itself!
    juce::Synthesiser Synth;

private:
    bool  Initialised         = false;
    float MinimumEnvelopeRate = 0.0001f;

    WaveType CurrentWaveType           = WaveType::Sin;
    float    CurrentAttackRateSeconds  = 0.01f;
	float    CurrentDecayRateSeconds   = 0.1f;
	float    CurrentSustainLevel       = 0.8f;
	float    CurrentReleaseRateSeconds = 0.1f;

    float CheckRate (float rate)
    {
        if (rate <= 0.0f)
        {
            UE_LOG(LogTemp, Warning, TEXT("Envelope rate cannot be 0. Converting to minimum rate."));
            return MinimumEnvelopeRate;
        }
        return rate;
    }

};