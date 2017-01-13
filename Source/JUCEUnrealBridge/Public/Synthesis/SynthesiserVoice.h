/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

/**
    Describes one of the sounds that a Synthesiser can play.

    A synthesiser can contain one or more sounds, and a sound can choose which
    midi notes and channels can trigger it.

    The USynthesiserSoundDescriptor is a passive class that just describes what 
    the sound is - the actual audio rendering for a sound is done by a 
    SynthesiserVoice. This allows more than one SynthesiserVoice to play the same 
    sound at the same time.
 
 	USynthesiserSoundDescriptor objects are not intended to be spawnable objects
 	within the game world. They are reference-counted and deleted by the synthesiser 
 	when no longer in use. 

    @see juce::Synthesiser, juce::SynthesiserVoice
*/

#pragma once

class USynthesiserSoundDescriptor : public juce::SynthesiserSound
{
protected:
    //==============================================================================
    FORCEINLINE USynthesiserSoundDescriptor() {}

public:
    /** Destructor. */
    FORCEINLINE virtual ~USynthesiserSoundDescriptor() {}

    //==============================================================================
    /** Returns true if this sound should be played when a given midi note is pressed.

        The Synthesiser will use this information when deciding which sounds to trigger
        for a given note.
    */
    FORCEINLINE virtual bool appliesToNote (int midiNoteNumber) override { return true; }

    /** Returns true if the sound should be triggered by midi events on a given channel.

        The Synthesiser will use this information when deciding which sounds to trigger
        for a given note.
    */
    FORCEINLINE virtual bool appliesToChannel (int midiChannel) override { return true; }

    /** The class is reference-counted, so this is a handy pointer class for it. */
    typedef juce::ReferenceCountedObjectPtr<juce::SynthesiserSound> Ptr;
};

//==============================================================================
/**
    Represents a voice that a Synthesiser can use to play a SynthesiserSound.

    A voice plays a single sound at a time, and a synthesiser holds an array of
    voices so that it can play polyphonically.

	USynthesiserVoice objects are not intended to be spawnable objects
 	within the game world. In general they should be instantiated when adding
 	them to a synthesiser, which will take ownership and delete them when no
 	longer needed.

    @see Synthesiser, SynthesiserSound
*/
class USynthesiserVoice : public juce::SynthesiserVoice
{
public:
    /**
    The default USynthesiserVoice plays default USynthesiserSoundDescriptor sound types. For more 
    complex synth setups, custom USynthesiserSoundDescriptor types can be defined. Custom synthesiser 
    voice classes would then check the sound descriptor type in this function. 
    */
    FORCEINLINE virtual bool canPlaySound (juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<USynthesiserSoundDescriptor*> (sound) != nullptr;
    }

    FORCEINLINE virtual void pitchWheelMoved (int /*newValue*/) override
    {
        // Respond to incoming MIDI pitch wheel messages
    }

    FORCEINLINE virtual void controllerMoved (int /*controllerNumber*/, int /*newValue*/) override
    {
        // Respond to incoming MIDI CC messages.
    }


    FORCEINLINE virtual void startNote (int midiNoteNumber, float velocity,
                    					juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {}

    FORCEINLINE virtual void stopNote (float /*velocity*/, bool allowTailOff) override
    {}

    /**
    The main audio render function, which is called continuously on the audio thread.
    */
    FORCEINLINE void renderNextBlock (juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {}

};