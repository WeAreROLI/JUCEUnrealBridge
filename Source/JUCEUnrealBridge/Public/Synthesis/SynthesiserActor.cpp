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
    AddOwnedComponent (SynthesiserComponent);
}

