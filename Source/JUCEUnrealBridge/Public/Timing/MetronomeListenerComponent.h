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

/** A Component that can register with a metronome and provide functionality to respond
    to rhythmic subdivision events (e.g. beat, eighth note, sixteenth note).

    The UMetronomeListenerComponent keeps track of the MetronomComponent to which it
    is registered and unregisters itself when detroyed.
*/
UCLASS(meta=(BlueprintSpawnableComponent), ClassGroup="JUCE-Components")
class JUCEUNREALBRIDGE_API UMetronomeListenerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMetronomeListenerComponent()
	{
		Listener.SetOwner (this);
        PrimaryComponentTick.bCanEverTick = true;
	}

    void OnComponentDestroyed (bool bDestroyingHierarchy) override
    {
        Listener.SetOwner (nullptr);
        if (MetronomeComponent != nullptr)
            DetachFromMetronome();
    }

    FORCEINLINE virtual void TickComponent (float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override
    {
        Listener.Tick();
    }

	UFUNCTION(BlueprintCallable, Category="JUCE-MetronomeListener")
	void RegisterWithMetronome (UMetronomeComponent* metronome)
	{
        DetachFromMetronome();
        if (metronome != nullptr)
        {
            MetronomeComponent = metronome;
		    MetronomeComponent->AddListener (&Listener);
        }
	}

	UPROPERTY(BlueprintAssignable, Category="JUCE-MetronomeListener")
    FMetronomeSixteenth OnSixteenth;
    UPROPERTY(BlueprintAssignable, Category="JUCE-MetronomeListener")
    FMetronomeEighth OnEighth;
    UPROPERTY(BlueprintAssignable, Category="JUCE-MetronomeListener")
    FMetronomeBeat OnBeat;
    UPROPERTY(BlueprintAssignable, Category="JUCE-MetronomeListener")
    FMetronomeBar OnBar;

    UPROPERTY(BlueprintReadonly, Category="JUCE-MetronomeListener")
    float LastSixteenthTime = 0.0f;
    UPROPERTY(BlueprintReadonly, Category="JUCE-MetronomeListener")
    float LastEighthTime = 0.0f;
    UPROPERTY(BlueprintReadonly, Category="JUCE-MetronomeListener")
    float LastBeatTime = 0.0f;
    UPROPERTY(BlueprintReadonly, Category="JUCE-MetronomeListener")
    float LastBarTime = 0.0f;

    UFUNCTION(BlueprintCallable, Category="JUCE-MetronomeListener")
    float GetTimeSinceLastSixteenth();
    UFUNCTION(BlueprintCallable, Category="JUCE-MetronomeListener")
    float GetTimeSinceLastEighth();
    UFUNCTION(BlueprintCallable, Category="JUCE-MetronomeListener")
    float GetTimeSinceLastBeat();
    UFUNCTION(BlueprintCallable, Category="JUCE-MetronomeListener")
    float GetTimeSinceLastBar();

    UMetronomeComponent* GetMetronomeComponent() { return MetronomeComponent; }

private:
    UPROPERTY(Transient)
    UMetronomeComponent* MetronomeComponent;

    void DetachFromMetronome()
	{
		if (MetronomeComponent != nullptr && MetronomeComponent->ContainsListener (&Listener))
			MetronomeComponent->RemoveListener (&Listener);
	}

	class MetronomeListener : public UMetronomeComponent::Listener
	{
	public:
		void SetOwner (UMetronomeListenerComponent* owner) { Owner = owner; }

		virtual void AsyncSixteenthCallback (int index) override;
        virtual void AsyncEighthCallback    (int index) override;
        virtual void AsyncBeatCallback      (int index) override;
        virtual void AsyncBarCallback       (int index) override;

	private:
		UMetronomeListenerComponent* Owner;
	};

	MetronomeListener Listener;
};