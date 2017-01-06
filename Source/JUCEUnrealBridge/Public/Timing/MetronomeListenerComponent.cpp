/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#include "JUCEUnrealBridgePCH.h"
#include "MetronomeListenerComponent.h"

void UMetronomeListenerComponent::MetronomeListener::SixteenthTicked (int index) 
{
	if (Owner != nullptr)
    {
        PendingCallback.set (1);
        AsyncTask (ENamedThreads::NormalTaskPriority, [this, index] () 
        {
            Owner->LastSixteenthTime = Owner->GetWorld()->GetTimeSeconds();
            Owner->OnSixteenth.Broadcast (index); 
            PendingCallback.set (0);
        });
    }
}

void UMetronomeListenerComponent::MetronomeListener::EighthTicked (int index)
{
	if (Owner != nullptr)
    {
        PendingCallback.set (1);
		AsyncTask (ENamedThreads::NormalTaskPriority, [this, index] () 
        { 
            Owner->LastEighthTime = Owner->GetWorld()->GetTimeSeconds();
            Owner->OnEighth.Broadcast (index); 
            PendingCallback.set (0);
        });
    }
}

void UMetronomeListenerComponent::MetronomeListener::BeatTicked (int index)
{
	if (Owner != nullptr)
    {
        PendingCallback.set (1);
        AsyncTask (ENamedThreads::NormalTaskPriority, [this, index] () 
        {
            Owner->LastBeatTime = Owner->GetWorld()->GetTimeSeconds();
            Owner->OnBeat.Broadcast (index); 
            PendingCallback.set (0);
        });
    }
}

void UMetronomeListenerComponent::MetronomeListener::BarTicked (int index)
{
	if (Owner != nullptr)
    {
        PendingCallback.set (1);
        AsyncTask (ENamedThreads::NormalTaskPriority, [this, index] () 
        { 
            Owner->LastBarTime = Owner->GetWorld()->GetTimeSeconds();
            Owner->OnBar.Broadcast (index); 
            PendingCallback.set (0);
        });
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