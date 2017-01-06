/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#pragma once

#include "UnrealADSREnvelope.h"
#include "UnrealWaveVoice.generated.h"

//================================================================================
UENUM (BlueprintType)
enum class WaveType : uint8
{
    Sin          UMETA(DisplayName = "Sin Wave"),
    Square 	     UMETA(DisplayName = "Squre Wave"),
	Pulse	     UMETA(DisplayName = "Pulse Wave"),
    Sawtooth     UMETA(DisplayName = "Sawtooth Wave"),
    NumWaveTypes UMETA(DisplayName = "Num Wave Types")
};
//================================================================================

class UnrealWaveVoice : public UnrealSynthesiserVoice
{
public:
	struct UnrealWaveSound : public UnrealSynthesiserSoundDescriptor {};

	FORCEINLINE UnrealWaveVoice() : currentAngle (0), angleDelta (0)
    {}

    FORCEINLINE void SetWaveformType (WaveType w) { WaveformType = w; }
    FORCEINLINE void SetAttackRateSeconds  (double rate)  { Envelope.SetAttackRateSeconds  (rate, getSampleRate()); }
    FORCEINLINE void SetDecayRateSeconds   (double rate)  { Envelope.SetDecayRateSeconds   (rate, getSampleRate()); }
    FORCEINLINE void SetReleaseRateSeconds (double rate)  { Envelope.SetReleaseRateSeconds (rate, getSampleRate()); }
	FORCEINLINE void SetSustainLevel       (double level) { Envelope.SetSustainLevel       (level); }
private:
    UPROPERTY(Transient)
    UADSREnvelope Envelope;

    UPROPERTY(Transient)
    WaveType WaveformType = WaveType::Sin;
	
    double currentAngle, angleDelta/*, level, tailOff*/;

	FORCEINLINE virtual bool canPlaySound (juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<UnrealWaveSound*> (sound) != nullptr;
    }

	FORCEINLINE virtual void startNote (int midiNoteNumber, float velocity,
                    					juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        currentAngle = 0.0;
        Envelope.TriggerEnvelopeStart();//level = velocity;//start envelope (envelope state = attack). / Gate enevelope
        //tailOff = 0.0;

        double cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber);
        double cyclesPerSample = cyclesPerSecond / getSampleRate();

        angleDelta = cyclesPerSample * 2.0 * juce::double_Pi;
    }
    
	FORCEINLINE virtual void stopNote (float /*velocity*/, bool allowTailOff) override
    {
        if (allowTailOff) //Gate envelope false??
        {
            Envelope.TriggerEnvelopeEnd();
            // start a tail-off by setting this flag. The render callback will pick up on
            // this and do a fade out, calling clearCurrentNote() when it's finished.

            //if (tailOff == 0.0) // we only need to begin a tail-off if it's not already doing so - the
            //                    // stopNote method could be called more than once.
            //    tailOff = 1.0;
        }
        else //Reset envelope??
        {
            Envelope.Reset();
            // we're being told to stop playing immediately, so reset everything..

            clearCurrentNote();
            angleDelta = 0.0;
        }
    }

    FORCEINLINE float getCurrentSample (const float pulseWidth)
    {
        switch (WaveformType)
        {
            case WaveType::Pulse:    return fmod (currentAngle, juce::float_Pi) < pulseWidth * juce::float_Pi ? 1.0f : 0.0f;
            case WaveType::Sawtooth: return fmod (currentAngle, juce::float_Pi) / juce::float_Pi;
            case WaveType::Square:   return fmod (currentAngle, 2.0f * juce::float_Pi) < juce::float_Pi ? 1.0f : -1.0f;
            default:
            case WaveType::Sin:      return (float) (std::sin (currentAngle));
        }
    }

    /**
    The main audio render function, which is called continuously on the audio thread.
    */
    FORCEINLINE void renderNextBlock (juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        if (angleDelta != 0.0)
        {
            const float pulseWidth = 0.2f;
            

            while (--numSamples >= 0)
            {
                const double level = Envelope.process();
                const float currentSample = getCurrentSample (pulseWidth)  * level;

                for (int i = outputBuffer.getNumChannels(); --i >= 0;)
                    outputBuffer.addSample (i, startSample, currentSample);

                currentAngle += angleDelta;
                ++startSample;

                if (Envelope.GetState() == EnvelopeState::Idle)
                {
                    clearCurrentNote();
                    angleDelta = 0.0;
                    break;
                }
            }

            //if (tailOff > 0)
            //{
            //    while (--numSamples >= 0)
            //    {
            //        const float currentSample = getCurrentSample (pulseWidth)  * level * tailOff;

            //        for (int i = outputBuffer.getNumChannels(); --i >= 0;)
            //            outputBuffer.addSample (i, startSample, currentSample);

            //        currentAngle += angleDelta;
            //        ++startSample;

            //        tailOff *= 0.99;

            //        if (tailOff <= 0.005)
            //        {
            //            clearCurrentNote();

            //            angleDelta = 0.0;
            //            break;
            //        }
            //    }
            //}
            //else
            //{
            //    while (--numSamples >= 0)
            //    {
            //        const float currentSample = getCurrentSample (pulseWidth) * level;

            //        for (int i = outputBuffer.getNumChannels(); --i >= 0;)
            //            outputBuffer.addSample (i, startSample, currentSample);

            //        currentAngle += angleDelta;
            //        ++startSample;
            //    }
            //}
        }
    }
};