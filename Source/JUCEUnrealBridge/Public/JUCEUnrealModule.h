/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#pragma once

DECLARE_LOG_CATEGORY_EXTERN (ModuleLog, Log, All)

class JUCEUnrealModule : public IJUCEUnrealBridge
{
private:
public:
	JUCEUnrealModule();

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

    
};