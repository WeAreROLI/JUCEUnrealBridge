/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#include "JUCEUnrealBridgePCH.h"
#include "SynthesiserActor.h"

ASynthesiserActor::ASynthesiserActor()
{
    SynthesiserComponent = CreateDefaultSubobject<USynthesiserComponent> (TEXT ("Synthesiser"));
}

void ASynthesiserActor::BeginPlay()
{
    Super::BeginPlay();
    
    AttackRateSeconds  = InitialAttackRateSeconds;
    DecayRateSeconds   = InitialDecayRateSeconds;
    SustainLevel       = InitialSustainLevel;
    ReleaseRateSeconds = InitialReleaseRateSeconds;

    SynthesiserComponent->SetAttackRateSeconds  ((double) AttackRateSeconds);
    SynthesiserComponent->SetDecayRateSeconds   ((double) DecayRateSeconds);
    SynthesiserComponent->SetSustainLevel       ((double) SustainLevel);
    SynthesiserComponent->SetReleaseRateSeconds ((double) ReleaseRateSeconds);

    AddOwnedComponent (SynthesiserComponent);
}

void ASynthesiserActor::SetAttackRateSeconds  (float newRate)  
{ 
	AttackRateSeconds = newRate; 
	SynthesiserComponent->SetAttackRateSeconds  ((double) AttackRateSeconds);  
}

void ASynthesiserActor::SetDecayRateSeconds   (float newRate)  
{ 
	DecayRateSeconds = newRate; 
	SynthesiserComponent->SetDecayRateSeconds   ((double) DecayRateSeconds);  
}

void ASynthesiserActor::SetSustainLevel       (float newLevel) 
{ 
	SustainLevel = newLevel; 
	SynthesiserComponent->SetSustainLevel       ((double) SustainLevel); 
}

void ASynthesiserActor::SetReleaseRateSeconds (float newRate)  
{ 
	ReleaseRateSeconds = newRate; 
	SynthesiserComponent->SetReleaseRateSeconds ((double) ReleaseRateSeconds);  
}

float ASynthesiserActor::GetAttackRateSeconds()  { return AttackRateSeconds;  }
float ASynthesiserActor::GetDecayRateSeconds()   { return DecayRateSeconds;   }
float ASynthesiserActor::GetSustainLevel()       { return SustainLevel;       }
float ASynthesiserActor::GetReleaseRateSeconds() { return ReleaseRateSeconds; }