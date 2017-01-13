/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI


	The following is a re-work of:
	http://www.earlevel.com/main/2013/06/03/envelope-generators-adsr-code/
	by Nigel Redmon
*/

#pragma once

#include "JUCEUnrealBridgePCH.h"
#include "ADSREnvelope.generated.h"

//================================================================================
/** indicates a specific temporal portion of an ADSR envelope.
    Idle indicates the envelope is 'at rest' not doing anything.
*/
UENUM (BlueprintType)
enum class EnvelopeState : uint8
{
    Idle    UMETA(DisplayName = "Idle"),
    Attack 	UMETA(DisplayName = "Attack"),
	Decay	UMETA(DisplayName = "Decay"),
    Sustain UMETA(DisplayName = "Sustain"),
    Release UMETA(DisplayName = "Release")
};
//================================================================================
/** A very simple ADSR envelope implementation. 
*/
class UADSREnvelope
{	
public:
	UADSREnvelope();

	FORCEINLINE double process() 
	{
		switch (EnvState) 
		{
	        case EnvelopeState::Idle: break;
	        case EnvelopeState::Attack:
	            Output = AttackBase + Output * AttackCoef;
	            if (Output >= 1.0) 
	            {
	                Output = 1.0;
	                EnvState = EnvelopeState::Decay;
	            }
	            break;
	        case EnvelopeState::Decay:
	            Output = DecayBase + Output * DecayCoef;
	            if (Output <= SustainLevel) 
	            {
	                Output = SustainLevel;
	                EnvState = EnvelopeState::Sustain;
	            }
	            break;
	        case EnvelopeState::Sustain: break;
	        case EnvelopeState::Release:
	            Output = ReleaseBase + Output * ReleaseCoef;
	            if (Output <= 0.0) 
	            {
	                Output = 0.0;
	                EnvState = EnvelopeState::Idle;
	            }
		}
		return Output;
	}

	FORCEINLINE void Reset() 
	{ 
		EnvState = EnvelopeState::Idle; 
		Output   = 0.0;
	}
	
	FORCEINLINE void TriggerEnvelopeEnd()  	
	{ 
		if (EnvState != EnvelopeState::Idle) 
			EnvState = EnvelopeState::Release; 
	}

	FORCEINLINE void TriggerEnvelopeStart() { EnvState = EnvelopeState::Attack; }
	FORCEINLINE EnvelopeState GetState()    { return EnvState; }
	FORCEINLINE double GetOutput() 			{ return Output; }

	void SetAttackRateSeconds  (double rate, double sampleRate);
    void SetDecayRateSeconds   (double rate, double sampleRate);
    void SetReleaseRateSeconds (double rate, double sampleRate);
	void SetSustainLevel       (double level);

	double GetAttackRateSeconds  (double sampleRate);
    double GetDecayRateSeconds   (double sampleRate);
    double GetReleaseRateSeconds (double sampleRate);
	double GetSustainLevel       ();

    void SetAttackTargetRatio       (double targetRatio);
    void SetDecayReleaseTargetRatio (double targetRatio);
    void UpdateCoefficientsAndBaseValues();

private:
	EnvelopeState EnvState = EnvelopeState::Idle;

	double Output                  = 0.0;
	double AttackRateSamples       = 4400.0;
	double DecayRateSamples        = 13200.0;
	double ReleaseRateSamples      = 13200.0;
	double AttackCoef              = 0.0;
	double DecayCoef               = 0.0;
	double ReleaseCoef             = 0.0;
	double SustainLevel            = 0.5;
    double AttackTargetRatio       = 0.3;
    double DecayReleaseTargetRatio = 0.0001;
    double AttackBase              = 0.0;
    double DecayBase               = 0.0;
    double ReleaseBase             = 0.0;

    void SetAttackRateSamples  (double rate);
    void SetDecayRateSamples   (double rate);
    void SetReleaseRateSamples (double rate);
    double CalcCoefficient     (double rate, double targetRatio);
};