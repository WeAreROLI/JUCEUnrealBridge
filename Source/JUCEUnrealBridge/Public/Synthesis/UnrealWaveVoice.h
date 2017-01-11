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

	FORCEINLINE UnrealWaveVoice() : currentAngle (0), angleDelta (0), level (0)
    {}

    FORCEINLINE void SetWaveformType (WaveType w) { WaveformType = w; }
    FORCEINLINE void SetAttackRateSeconds  (double rate)     { Envelope.SetAttackRateSeconds  (rate, getSampleRate()); }
    FORCEINLINE void SetDecayRateSeconds   (double rate)     { Envelope.SetDecayRateSeconds   (rate, getSampleRate()); }
    FORCEINLINE void SetReleaseRateSeconds (double rate)     { Envelope.SetReleaseRateSeconds (rate, getSampleRate()); }
	FORCEINLINE void SetSustainLevel       (double susLevel) { Envelope.SetSustainLevel       (susLevel); }
private:
    //UPROPERTY(Transient)
    UADSREnvelope Envelope;

    //UPROPERTY(Transient)
    WaveType WaveformType = WaveType::Sin;
	
    double currentAngle, angleDelta, level;

	FORCEINLINE virtual bool canPlaySound (juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<UnrealWaveSound*> (sound) != nullptr;
    }

	FORCEINLINE virtual void startNote (int midiNoteNumber, float velocity,
                    					juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        currentAngle = 0.0;
        level = (double) velocity;
        Envelope.TriggerEnvelopeStart();

        double cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber);
        double cyclesPerSample = cyclesPerSecond / getSampleRate();

        angleDelta = cyclesPerSample * 2.0 * juce::double_Pi;
    }
    
	FORCEINLINE virtual void stopNote (float /*velocity*/, bool allowTailOff) override
    {
        if (allowTailOff)
        {
            Envelope.TriggerEnvelopeEnd();
        }
        else
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
                const double envLevel = Envelope.process();
                const float currentSample = getCurrentSample (pulseWidth)  * envLevel * level;

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
        }
    }
};