/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#pragma once

#include "JUCEUnrealBridgePCH.h"
#include "TimeTickerComponent.h"
#include "TimeTickerActor.generated.h"

UCLASS()
class JUCEUNREALBRIDGE_API ATimeTickerActor : public AActor
{
    GENERATED_BODY()

public:
    ATimeTickerActor();
    void BeginPlay() override;

    UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "JUCE-AudioCallback")
    UTimeTickerComponent* TimeTickerComponent;

    virtual void TickCallback();

private:

};