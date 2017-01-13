/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI

	The following is a re-work of:
	http://www.earlevel.com/main/2013/06/03/envelope-generators-adsr-code/
	by Nigel Redmon
*/

#include "JUCEUnrealBridgePCH.h"
#include "ADSREnvelope.h"

UADSREnvelope::UADSREnvelope()
{
	UpdateCoefficientsAndBaseValues();
}

void UADSREnvelope::SetAttackRateSeconds (double rate, double sampleRate)
{
    SetAttackRateSamples (rate * sampleRate);
}

void UADSREnvelope::SetDecayRateSeconds (double rate, double sampleRate)
{
	SetDecayRateSamples (rate * sampleRate);
}

void UADSREnvelope::SetReleaseRateSeconds (double rate, double sampleRate)
{
	SetReleaseRateSamples (rate * sampleRate);
}

void UADSREnvelope::SetAttackRateSamples (double rate)
{
	AttackRateSamples = rate;
	AttackCoef = CalcCoefficient (rate, AttackTargetRatio);
	AttackBase = (1.0 + AttackTargetRatio) * (1.0 - AttackCoef);
}

void UADSREnvelope::SetDecayRateSamples (double rate)
{
	DecayRateSamples = rate;
	DecayCoef = CalcCoefficient (rate, DecayReleaseTargetRatio);
	DecayBase = (SustainLevel + DecayReleaseTargetRatio) * (1.0 - DecayCoef);
}

void UADSREnvelope::SetReleaseRateSamples (double rate)
{
	ReleaseRateSamples = rate;
	ReleaseCoef = CalcCoefficient (rate, DecayReleaseTargetRatio);
	ReleaseBase = -DecayReleaseTargetRatio * (1.0 - ReleaseCoef);
}

void UADSREnvelope::SetSustainLevel (double level)
{
	SustainLevel = level;
	DecayBase = (SustainLevel - DecayReleaseTargetRatio) * (1.0 - DecayCoef);
}

double UADSREnvelope::GetAttackRateSeconds (double sampleRate)
{
	check (sampleRate != 0.0);
    return AttackRateSamples / sampleRate;
}

double UADSREnvelope::GetDecayRateSeconds (double sampleRate)
{
	check (sampleRate != 0.0);
	return DecayRateSamples / sampleRate;
}

double UADSREnvelope::GetReleaseRateSeconds (double sampleRate)
{
	check (sampleRate != 0.0);
	return ReleaseRateSamples / sampleRate;
}

double UADSREnvelope::GetSustainLevel()
{
	return SustainLevel;
}

void UADSREnvelope::SetAttackTargetRatio (double targetRatio)
{
	if (targetRatio < 0.000000001)
        targetRatio = 0.000000001;  // -180 dB
    AttackTargetRatio = targetRatio;
    AttackCoef = CalcCoefficient (AttackRateSamples, AttackTargetRatio);
    AttackBase = (1.0 + AttackTargetRatio) * (1.0 - AttackCoef);
}

void UADSREnvelope::SetDecayReleaseTargetRatio (double targetRatio)
{
	if (targetRatio < 0.000000001)
        targetRatio = 0.000000001;  // -180 dB
    
    DecayReleaseTargetRatio = targetRatio;

    DecayCoef   = CalcCoefficient (DecayRateSamples,   DecayReleaseTargetRatio);
    ReleaseCoef = CalcCoefficient (ReleaseRateSamples, DecayReleaseTargetRatio);

    DecayBase   = (SustainLevel - DecayReleaseTargetRatio) * (1.0 - DecayCoef);
    ReleaseBase = -DecayReleaseTargetRatio * (1.0 - ReleaseCoef);
}

double UADSREnvelope::CalcCoefficient (double rate, double targetRatio)
{
	check (rate != 0.0);
	return exp(-log((1.0 + targetRatio) / targetRatio) / rate);
}

void UADSREnvelope::UpdateCoefficientsAndBaseValues()
{
	AttackCoef = CalcCoefficient (AttackRateSamples, AttackTargetRatio);
	AttackBase = (1.0 + AttackTargetRatio) * (1.0 - AttackCoef);

	DecayCoef = CalcCoefficient (DecayRateSamples, DecayReleaseTargetRatio);
	DecayBase = (SustainLevel + DecayReleaseTargetRatio) * (1.0 - DecayCoef);

	ReleaseCoef = CalcCoefficient (ReleaseRateSamples, DecayReleaseTargetRatio);
	ReleaseBase = -DecayReleaseTargetRatio * (1.0 - ReleaseCoef);

	DecayBase = (SustainLevel - DecayReleaseTargetRatio) * (1.0 - DecayCoef);
}