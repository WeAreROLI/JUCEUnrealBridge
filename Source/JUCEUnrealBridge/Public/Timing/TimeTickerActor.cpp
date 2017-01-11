/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#include "JUCEUnrealBridgePCH.h"
#include "TimeTickerActor.h"

ATimeTickerActor::ATimeTickerActor()
{
    TimeTickerComponent = CreateDefaultSubobject<UTimeTickerComponent> (TEXT ("TimeTicker"));
}

void ATimeTickerActor::BeginPlay()
{
    Super::BeginPlay();
    
    if (TimeTickerComponent != nullptr)
    {
    	TimeTickerComponent->SetTickCallback([this]()
    	{
    		TickCallback();
    	});
    	AddOwnedComponent (TimeTickerComponent);
        TimeTickerComponent->StartAudio();
    }
}

void ATimeTickerActor::TickCallback()
{}