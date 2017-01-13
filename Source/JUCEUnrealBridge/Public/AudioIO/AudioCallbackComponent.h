/*
	JUCE Unreal Bridge 0.0.1
	----
	Sean Soraghan - ROLI
*/

#pragma once

#include "JUCEUnrealBridgePCH.h"
#include "AudioCallbackComponent.generated.h"

/**
This Component is a wrapper around a juce::AudioIODeviceCallback object and acts as an audio IO responder for the global juce::AudioDeviceManager instance.
*/
UCLASS()
class UAudioCallbackComponent : public UActorComponent
{
	GENERATED_BODY()

private:
    /** The inner JUCE AudioDeviceIOCallback class 
    */
    class UAudioDeviceCallback : public juce::AudioIODeviceCallback
    {
    public:
	    void audioDeviceIOCallback (const float **inputChannelData, int numInputChannels, 
	    						    float **outputChannelData, int numOutputChannels, int numSamples) override;
	    void audioDeviceAboutToStart (juce::AudioIODevice *device) override;
	    void audioDeviceStopped() override;

        void setAudioDeviceIOCallback (std::function<void (const float **inputChannelData, int numInputChannels, 
	    						       float **outputChannelData, int numOutputChannels, int numSamples)> func);

        void setAudioDeviceAboutToStartCallback (std::function<void (juce::AudioIODevice *device)> func);
        void setAudioDeviceStoppedCallback      (std::function<void ()> func);

    private:
        std::function<void (const float **inputChannelData, int numInputChannels, 
	    				    float **outputChannelData, int numOutputChannels, int numSamples)> unrealAudioDeviceIOCallback;
        std::function<void (juce::AudioIODevice *device)>                                      unrealAudioDeviceAboutToStartCallback;
	    std::function<void()>                                                                  unrealAudioDeviceStoppedCallback;
    };

public:
    UAudioCallbackComponent();
    void InitializeComponent() override;
    void OnComponentDestroyed (bool bDestroyingHierarchy) override;

    double GetSampleRate();
   
    /** This should be called by the owning actor during begin play.
    */
    UFUNCTION(BlueprintCallable, Category = "JUCE-AudioCallbackComponent")
    void StartAudio();
    /** This will be called on component destruction.
    */
    UFUNCTION(BlueprintCallable, Category = "JUCE-AudioCallbackComponent")
    void StopAudio();

protected:
    /** Called to indicate that the device is about to start calling back.

        This will be called just before the audio callbacks begin, either when this
        callback has just been added to an audio device, or after the device has been
        restarted because of a sample-rate or block-size change.

        You can use this opportunity to find out the sample rate and block size
        that the device is going to use by calling the AudioIODevice::getCurrentSampleRate()
        and AudioIODevice::getCurrentBufferSizeSamples() on the supplied pointer.

        @param device       the audio IO device that will be used to drive the callback.
                            Note that if you're going to store this this pointer, it is
                            only valid until the next time that audioDeviceStopped is called.
    */
    virtual void AudioDeviceAboutToStart (juce::AudioIODevice *device) {}
    
    /** Called to indicate that the device has stopped. */
    virtual void AudioDeviceStopped() {}
    
    /** Processes a block of incoming and outgoing audio data.

        The subclass's implementation should use the incoming audio for whatever
        purposes it needs to, and must fill all the output channels with the next
        block of output data before returning.

        The channel data is arranged with the same array indices as the channel name
        array returned by AudioIODevice::getOutputChannelNames(), but those channels
        that aren't specified in AudioIODevice::open() will have a null pointer for their
        associated channel, so remember to check for this.

        @param inputChannelData     a set of arrays containing the audio data for each
                                    incoming channel - this data is valid until the function
                                    returns. There will be one channel of data for each input
                                    channel that was enabled when the audio device was opened
                                    (see AudioIODevice::open())
        @param numInputChannels     the number of pointers to channel data in the
                                    inputChannelData array.
        @param outputChannelData    a set of arrays which need to be filled with the data
                                    that should be sent to each outgoing channel of the device.
                                    There will be one channel of data for each output channel
                                    that was enabled when the audio device was opened (see
                                    AudioIODevice::open())
                                    The initial contents of the array is undefined, so the
                                    callback function must fill all the channels with zeros if
                                    its output is silence. Failing to do this could cause quite
                                    an unpleasant noise!
        @param numOutputChannels    the number of pointers to channel data in the
                                    outputChannelData array.
        @param numSamples           the number of samples in each channel of the input and
                                    output arrays. The number of samples will depend on the
                                    audio device's buffer size and will usually remain constant,
                                    although this isn't guaranteed, so make sure your code can
                                    cope with reasonable changes in the buffer size from one
                                    callback to the next.
    */
    virtual void AudioDeviceIOCallback (const float **inputChannelData, int numInputChannels, 
                                        float **outputChannelData, int numOutputChannels, int numSamples) {}

private:
    UAudioDeviceCallback deviceCallback;

    void RegisterAudioCallback();
    void UnregisterAudioCallback();
};