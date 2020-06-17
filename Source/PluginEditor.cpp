/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TestPluginAudioProcessorEditor::TestPluginAudioProcessorEditor (TestPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (WIDTH, HEIGHT);
	
	addAndMakeVisible(&m_OpenButton);
	m_OpenButton.setButtonText("Open...");
	m_OpenButton.onClick = [this] { OpenButtonClicked(); };

	addAndMakeVisible(&m_PlayButton);
	m_PlayButton.setButtonText("Play");
	m_PlayButton.onClick = [this] { PlayButtonClicked(); };
	m_PlayButton.setColour(TextButton::buttonColourId, Colours::green);
	m_PlayButton.setEnabled(false);

	addAndMakeVisible(&m_StopButton);
	m_StopButton.setButtonText("Stop");
	m_StopButton.onClick = [this] { StopButtonClicked(); };
	m_StopButton.setColour(TextButton::buttonColourId, Colours::red);
	m_StopButton.setEnabled(false);

	processor.m_TransportSource.addChangeListener(this);

	//InitMidiVolumeSlier();
	processor.m_NoiseMix.CreateEditor(this);
	processor.m_NoiseMix.GetEditor()->Init();

	processor.m_DelayGain.CreateEditor(this);
	processor.m_DelayGain.GetEditor()->Init();

	processor.m_DelayTime.CreateEditor(this);
	processor.m_DelayTime.GetEditor()->Init();
	processor.m_DelayTime.GetEditor()->m_OnValueChangedFuntion = [this](Slider*) { processor.UpdateDelayTime(); };

	addAndMakeVisible(processor.m_TimeTypeButton);
	//processor.m_TestButton.setButtonText("TEST");
	processor.m_TimeTypeButton.setColour(ToggleButton::tickDisabledColourId, Colours::forestgreen);
	processor.m_TimeTypeButton.setColour(ToggleButton::tickColourId, Colours::green);
	processor.m_TimeTypeButton.onClick = [this]
	{
		processor.m_bTimeFromBPM = !processor.m_bTimeFromBPM;
		processor.UpdateDelayTime();
	};
	//processor.m_TestButton.setColour(ToggleButton::textColourId, Colours::black);
	//processor.m_TestButton.setEnabled(false);

	resized();
}

TestPluginAudioProcessorEditor::~TestPluginAudioProcessorEditor()
{
}

//==============================================================================
void TestPluginAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

	// fill the whole window white
	g.fillAll(Colours::white);

	// set the current drawing colour to black
	g.setColour(Colours::black);

	// set the font size and draw text to the screen
	g.setFont(15.0f);

	g.drawFittedText("Midi Volume", 0, 0, getWidth(), 30, Justification::centred, 1);
}

void TestPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

	m_OpenButton.setBounds(10, 10, 20, 20);
	m_PlayButton.setBounds(40, 10, 20, 20);
	m_StopButton.setBounds(70, 10, 20, 20);

	// sets the position and size of the slider with arguments (x, y, width, height)
	if(processor.m_NoiseMix.HasEditor())
		processor.m_NoiseMix.GetEditor()->Resize(getWidth() - 80, getHeight() - 80, 60, 60);
	if (processor.m_DelayGain.HasEditor())
		processor.m_DelayGain.GetEditor()->Resize(getWidth() - 160, getHeight() - 80, 60, 60);
	if (processor.m_DelayTime.HasEditor())
		processor.m_DelayTime.GetEditor()->Resize(getWidth() - 240, getHeight() - 80, 60, 60);

	m_MidiVolumeSlider.setBounds(40, 30, 20, getHeight() - 60);

	processor.m_TimeTypeButton.setBounds(getWidth() - 218, getHeight() - 20, 14, 14);
}

void TestPluginAudioProcessorEditor::ChangeState(TestPluginAudioProcessor::TransportState NewState)
{
	if (processor.m_State != NewState)
	{
		processor.m_State = NewState;

		switch (processor.m_State)
		{
		case TestPluginAudioProcessor::Stopped:                           // [3]
			m_StopButton.setEnabled(false);
			m_PlayButton.setEnabled(true);
			processor.m_TransportSource.setPosition(0.0);
			break;

		case TestPluginAudioProcessor::Starting:                          // [4]
			m_PlayButton.setEnabled(false);
			processor.m_TransportSource.start();
			break;

		case TestPluginAudioProcessor::Playing:                           // [5]
			m_StopButton.setEnabled(true);
			break;

		case TestPluginAudioProcessor::Stopping:                          // [6]
			processor.m_TransportSource.stop();
			break;
		}
	}
}

void TestPluginAudioProcessorEditor::OpenButtonClicked()
{
	FileChooser chooser("Select a Wave file to play...",
		{},
		"*.wav");                                        // [7]

	if (chooser.browseForFileToOpen())                                    // [8]
	{
		auto file = chooser.getResult();                                  // [9]
		auto* reader = processor.m_FormatManager.createReaderFor(file);              // [10]
		
		if (reader != nullptr)
		{
			std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true)); // [11]
			processor.m_TransportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);                     // [12]
			m_PlayButton.setEnabled(true);                                                                    // [13]
			processor.m_ReaderSource.reset(newSource.release());                                                        // [14]
		}
	}
}

void TestPluginAudioProcessorEditor::PlayButtonClicked()
{
	ChangeState(TestPluginAudioProcessor::Starting);
}

void TestPluginAudioProcessorEditor::StopButtonClicked()
{
	ChangeState(TestPluginAudioProcessor::Stopping);
}

/*
void TestPluginAudioProcessorEditor::InitMixKnob()
{
	m_MixKnob.setSliderStyle(Slider::Rotary);
	m_MixKnob.setRange(0.0, 1.0, 0.01);
	m_MixKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 90, 20);
	m_MixKnob.setPopupDisplayEnabled(true, false, this);
	m_MixKnob.setTextValueSuffix(" Mix");
	m_MixKnob.setValue(1.0);

	addAndMakeVisible(&m_MixKnob);
	m_MixKnob.addListener(this);
}
*/

void TestPluginAudioProcessorEditor::InitMidiVolumeSlier()
{
	// these define the parameters of our slider object
	m_MidiVolumeSlider.setSliderStyle(Slider::LinearBarVertical);
	m_MidiVolumeSlider.setRange(0.0, 127.0, 1.0);
	m_MidiVolumeSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	m_MidiVolumeSlider.setPopupDisplayEnabled(true, false, this);
	m_MidiVolumeSlider.setTextValueSuffix(" Volume");
	m_MidiVolumeSlider.setValue(127.0);

	// this function adds the slider to the editor
	addAndMakeVisible(&m_MidiVolumeSlider);
	// add the listener to the slider
	m_MidiVolumeSlider.addListener(this);
}

void TestPluginAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	processor.m_NoteOnVel = m_MidiVolumeSlider.getValue();
	//processor.m_Mix = m_MixKnob.getValue();
}

void TestPluginAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == &processor.m_TransportSource)
	{
		if (processor.m_TransportSource.isPlaying())
			ChangeState(TestPluginAudioProcessor::Playing);
		else
			ChangeState(TestPluginAudioProcessor::Stopped);
	}
}
