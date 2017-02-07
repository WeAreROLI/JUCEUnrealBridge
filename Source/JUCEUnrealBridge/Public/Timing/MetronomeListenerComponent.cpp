/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#include "JUCEUnrealBridgePCH.h"
#include "MetronomeListenerComponent.h"

void UMetronomeListenerComponent::MetronomeListener::SixteenthCallback (int index) 
{
	if (Owner != nullptr)
    {
        Owner->LastSixteenthTime = Owner->GetWorld()->GetTimeSeconds();
        Owner->OnSixteenth.Broadcast (index); 
    }
}

void UMetronomeListenerComponent::MetronomeListener::EighthCallback (int index)
{
	if (Owner != nullptr)
    {
        Owner->LastEighthTime = Owner->GetWorld()->GetTimeSeconds();
        Owner->OnEighth.Broadcast (index); 
    }
}

void UMetronomeListenerComponent::MetronomeListener::BeatCallback (int index)
{
	if (Owner != nullptr)
    {
        Owner->LastBeatTime = Owner->GetWorld()->GetTimeSeconds();
        Owner->OnBeat.Broadcast (index); 
    }
}

void UMetronomeListenerComponent::MetronomeListener::BarCallback (int index)
{
	if (Owner != nullptr)
    {
        Owner->LastBarTime = Owner->GetWorld()->GetTimeSeconds();
        Owner->OnBar.Broadcast (index); 
    }
}

float UMetronomeListenerComponent::GetTimeSinceLastSixteenth() { return GetWorld()->GetTimeSeconds() - LastSixteenthTime; }
float UMetronomeListenerComponent::GetTimeSinceLastEighth()    { return GetWorld()->GetTimeSeconds() - LastEighthTime;    }
float UMetronomeListenerComponent::GetTimeSinceLastBeat()      { return GetWorld()->GetTimeSeconds() - LastBeatTime;      }
float UMetronomeListenerComponent::GetTimeSinceLastBar()       { return GetWorld()->GetTimeSeconds() - LastBarTime;       }

void ReceiveSixteenthTick_Implementation() {}
void ReceiveEigthTick_Implementation()     {}
void ReceiveBeatTick_Implementation()      {}
void ReceiveBarTick_Implementation()       {}