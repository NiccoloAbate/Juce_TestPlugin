/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class TestPluginAudioProcessorEditor  : public AudioProcessorEditor, private Slider::Listener, private ChangeListener
{
public:
    TestPluginAudioProcessorEditor (TestPluginAudioProcessor&);
    ~TestPluginAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

	void ChangeState(TestPluginAudioProcessor::TransportState NewState);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TestPluginAudioProcessor& processor;

	static constexpr int WIDTH = 300;
	static constexpr int HEIGHT = 300;

	TextButton m_OpenButton;
	void OpenButtonClicked();
	TextButton m_PlayButton;
	void PlayButtonClicked();
	TextButton m_StopButton;
	void StopButtonClicked();

	//Slider m_MixKnob;
	//void InitMixKnob();

	Slider m_MidiVolumeSlider;
	void InitMidiVolumeSlier();

	void sliderValueChanged(Slider* slider) override;
	void changeListenerCallback(ChangeBroadcaster* source) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TestPluginAudioProcessorEditor)
};
