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
        AsyncTask (ENamedThreads::GameThread, [this, index] () { Owner->OnSixteenth.Broadcast (index); });
}

void UMetronomeListenerComponent::MetronomeListener::EighthTicked (int index)
{
	if (Owner != nullptr)
		AsyncTask (ENamedThreads::GameThread, [this, index] () { Owner->OnEighth.Broadcast (index); });
}

void UMetronomeListenerComponent::MetronomeListener::BeatTicked (int index)
{
	if (Owner != nullptr)
        AsyncTask (ENamedThreads::GameThread, [this, index] () { Owner->OnBeat.Broadcast (index); });
}

void UMetronomeListenerComponent::MetronomeListener::BarTicked (int index)
{
	if (Owner != nullptr)
        AsyncTask (ENamedThreads::GameThread, [this, index] () { Owner->OnBar.Broadcast (index); });
}

void ReceiveSixteenthTick_Implementation() {}
void ReceiveEigthTick_Implementation()     {}
void ReceiveBeatTick_Implementation()      {}
void ReceiveBarTick_Implementation()       {}