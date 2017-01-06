/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#pragma once

#include "JUCEUnrealBridgePCH.h"
#include "Async.h"
#include "MetronomeComponent.h"
#include "MetronomeListenerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMetronomeSixteenth, int, index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMetronomeEighth,    int, index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMetronomeBeat,      int, index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMetronomeBar,       int, index);

UCLASS(meta=(BlueprintSpawnableComponent), ClassGroup="JUCE-Components")
class JUCEUNREALBRIDGE_API UMetronomeListenerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMetronomeListenerComponent()
	{
		Listener.SetOwner (this);
	}

	UFUNCTION(BlueprintCallable, Category="JUCE-MetronomeListener")
	void RegisterWithMetronome (UMetronomeComponent* metronome)
	{
		metronome->AddListener (&Listener);
	}

	UFUNCTION(BlueprintCallable, Category="JUCE-MetronomeListener")
	void DetachFromMetronome (UMetronomeComponent* metronome)
	{
		if (metronome->ContainsListener (&Listener))
			metronome->RemoveListener (&Listener);
	}

	UPROPERTY(BlueprintAssignable, Category="JUCE-Metronome")
    FMetronomeSixteenth OnSixteenth;
    UPROPERTY(BlueprintAssignable, Category="JUCE-Metronome")
    FMetronomeEighth OnEighth;
    UPROPERTY(BlueprintAssignable, Category="JUCE-Metronome")
    FMetronomeBeat OnBeat;
    UPROPERTY(BlueprintAssignable, Category="JUCE-Metronome")
    FMetronomeBar OnBar;

private:
	class MetronomeListener : public UMetronomeComponent::Listener
	{
	public:
		void SetOwner (UMetronomeListenerComponent* owner) { Owner = owner; }

		virtual void SixteenthTicked (int index) override;
        virtual void EighthTicked     (int index) override;
        virtual void BeatTicked      (int index) override;
        virtual void BarTicked       (int index) override;

	private:
		UMetronomeListenerComponent* Owner;
	};

	MetronomeListener Listener;
};