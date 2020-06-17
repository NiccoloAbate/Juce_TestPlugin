/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "Delay/DelayBuffer.h"
#include "Delay/CircularAudioBuffer.h"
#include "PluginComponents/MixComponent.h"
#include "PluginComponents/GainComponent.h"
#include "PluginComponents/Parameter.h"
#include "Delay/GrainManager.h"



//==============================================================================
/**
*/
class TestPluginAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    TestPluginAudioProcessor();
    ~TestPluginAudioProcessor();

	float m_NoteOnVel = 127.0;

	MixComponent m_NoiseMix = MixComponent(0.0);

	GrainManager<float> m_GrainManager = GrainManager<float>(INITIALIZATION_SAMPLERATE * MAX_DELAYTIME, 2, 2);

	CircularAudioBuffer<float> m_DelayBuffer = CircularAudioBuffer<float>(INITIALIZATION_SAMPLERATE * MAX_DELAYTIME, 2, 0);
	static constexpr float MAX_DELAYTIME = 1.0f;
	static constexpr int INITIALIZATION_SAMPLERATE = 44100;
	GainComponent m_DelayGain = GainComponent(-10, GainComponent::Decibel);
	ParameterComponent<float> m_DelayTime = ParameterComponent<float>(0.5f);

	ToggleButton m_TimeTypeButton;
	bool m_bTimeFromBPM = false;

	void UpdateDelayTime();
	juce::AudioPlayHead::CurrentPositionInfo m_LastResult;
	void CheckPlayHeadPosition();

	enum TransportState
	{
		Stopped,
		Starting,
		Playing,
		Stopping
	};

	AudioFormatManager m_FormatManager;
	std::unique_ptr<AudioFormatReaderSource> m_ReaderSource;
	AudioTransportSource m_TransportSource;
	TransportState m_State;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TestPluginAudioProcessor)
};
