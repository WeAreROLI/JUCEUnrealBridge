/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#pragma once

#include "JUCEUnrealBridgePCH.h"
#include "AudioSourceComponent.generated.h"

/**
This Component is a wrapper around a juce::AudioSource object and acts as an audio source for the global juce::AudioDeviceManager instance.
*/
UCLASS()
class UAudioSourceComponent : public UActorComponent
{
	GENERATED_BODY()

private:
    /** The inner JUCE AudioSource class
    */
    class UAudioSource : public juce::AudioSource
    {
    public:
	    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
	    void releaseResources() override;
        void getNextAudioBlock           (const juce::AudioSourceChannelInfo& bufferToFill) override;
	    
        void setGetNextBufferCallback    (std::function<void (const juce::AudioSourceChannelInfo& bufferToFill)> func);
        void setPrepareToPlayCallback    (std::function<void (int samplesPerBlockExpected, double sampleRate)> func);
        void setReleaseResourcesCallback (std::function<void()> func);

	    double getSampleRate();
	    int    getSamplesPerBlockExpected();

    private:
    	int    samplesPerBlockExpected {0};
    	double sampleRate 			   {0.0};
        std::function<void (const juce::AudioSourceChannelInfo& bufferToFill)> unrealGetNextAudioBlockCallback;
        std::function<void (int samplesPerBlockExpected, double sampleRate)>   unrealPrepareToPlayCallback;
        std::function<void()>                                                  unrealReleaseResourcesCallback;
    };

public: 
    UAudioSourceComponent();
    virtual void InitializeComponent() override;
    
    /** Inherriting classes should call this function during initialisation.
    */
    UFUNCTION(BlueprintCallable, Category = "JUCE-AudioCallbackComponent")
    void StartAudio();

    /** This will be called when component is destroyed.
    */
    UFUNCTION(BlueprintCallable, Category = "JUCE-AudioCallbackComponent")
    void StopAudio();
    
    double GetSampleRate();

    void   OnComponentDestroyed (bool bDestroyingHierarchy) override;

protected:
    /** Tells the source to prepare for playing.

        An AudioSource has two states: prepared and unprepared.

        The prepareToPlay() method is guaranteed to be called at least once on an 'unpreprared'
        source to put it into a 'prepared' state before any calls will be made to getNextAudioBlock().
        This callback allows the source to initialise any resources it might need when playing.

        Once playback has finished, the releaseResources() method is called to put the stream
        back into an 'unprepared' state.

        Note that this method could be called more than once in succession without
        a matching call to releaseResources(), so make sure your code is robust and
        can handle that kind of situation.

        @param samplesPerBlockExpected  the number of samples that the source
                                        will be expected to supply each time its
                                        getNextAudioBlock() method is called. This
                                        number may vary slightly, because it will be dependent
                                        on audio hardware callbacks, and these aren't
                                        guaranteed to always use a constant block size, so
                                        the source should be able to cope with small variations.
        @param sampleRate               the sample rate that the output will be used at - this
                                        is needed by sources such as tone generators.
        @see ReleaseResources, GetNextAudioBlock
	*/
    FORCEINLINE virtual void PrepareToPlay     (int samplesPerBlockExpected, double sampleRate) {}
    /** Allows the source to release anything it no longer needs after playback has stopped.

	    This will be called when the source is no longer going to have its getNextAudioBlock()
	    method called, so it should release any spare memory, etc. that it might have
	    allocated during the prepareToPlay() call.

	    Note that there's no guarantee that prepareToPlay() will actually have been called before
	    releaseResources(), and it may be called more than once in succession, so make sure your
	    code is robust and doesn't make any assumptions about when it will be called.

	    @see PrepareToPlay, GetNextAudioBlock
	*/
    FORCEINLINE virtual void ReleaseResources() {}
    /** Called repeatedly to fetch subsequent blocks of audio data.

	    After calling the prepareToPlay() method, this callback will be made each
	    time the audio playback hardware (or whatever other destination the audio
	    data is going to) needs another block of data.

	    It will generally be called on a high-priority system thread, or possibly even
	    an interrupt, so be careful not to do too much work here, as that will cause
	    audio glitches!

	    @see juce::AudioSourceChannelInfo, PrepareToPlay, ReleaseResources
	*/
    FORCEINLINE virtual void GetNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) {}
    
private:
    UAudioSource            audioSource;
    juce::AudioSourcePlayer audioSourcePlayer;

    void   RegisterAudioSource();
    void   UnregisterAudioSource();
};
