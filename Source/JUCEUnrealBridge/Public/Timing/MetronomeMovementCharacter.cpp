/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#include "JUCEUnrealBridgePCH.h"
#include "MetronomeMovementCharacter.h"

AMetronomeMovementCharacter::AMetronomeMovementCharacter()
{
    MetronomeListenerComponent = CreateDefaultSubobject<UMetronomeListenerComponent> (TEXT ("MetronomeListener"));
}

void AMetronomeMovementCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    AddOwnedComponent (MetronomeListenerComponent);
}

void AMetronomeMovementCharacter::EndPlay (const EEndPlayReason::Type endPlayReason)
{
    Super::EndPlay (endPlayReason);
    if (MetronomeComponent != nullptr)
        DetachFromMetronome (MetronomeComponent);
}