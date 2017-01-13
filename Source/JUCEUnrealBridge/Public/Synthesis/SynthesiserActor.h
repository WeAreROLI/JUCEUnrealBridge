/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#pragma once

#include "JUCEUnrealBridgePCH.h"
#include "SynthesiserComponent.h"
#include "SynthesiserActor.generated.h"

/** An actor that has a synthesiser component. 
*/
UCLASS()
class JUCEUNREALBRIDGE_API ASynthesiserActor : public AActor
{
    GENERATED_BODY()

public:
	ASynthesiserActor();
	virtual void BeginPlay() override;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "JUCE-MetronomeMovement")
    USynthesiserComponent* SynthesiserComponent;
};