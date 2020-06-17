/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "Utils/BufferOperations.h"

//==============================================================================
TestPluginAudioProcessor::TestPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	m_FormatManager.registerBasicFormats();
}

TestPluginAudioProcessor::~TestPluginAudioProcessor()
{
}

void TestPluginAudioProcessor::UpdateDelayTime()
{
	if (m_bTimeFromBPM)
		m_DelayBuffer.SetOffset((60.0f / m_LastResult.bpm) * m_DelayTime.m_Param * getSampleRate());
	else
		m_DelayBuffer.SetOffset(m_DelayTime.m_Param * getSampleRate());
}

void TestPluginAudioProcessor::CheckPlayHeadPosition()
{
	juce::AudioPlayHead::CurrentPositionInfo Result;
	this->getPlayHead()->getCurrentPosition(Result);

	if (Result.bpm != m_LastResult.bpm)
	{
		if (m_bTimeFromBPM)
			m_DelayBuffer.SetOffset((m_LastResult.bpm / 60.0f) * 0.5f * m_DelayTime.m_Param * getSampleRate());

		m_LastResult = Result;
	}
}

//==============================================================================
const String TestPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TestPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TestPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TestPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TestPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TestPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TestPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TestPluginAudioProcessor::setCurrentProgram (int index)
{
}

const String TestPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void TestPluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void TestPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	m_TransportSource.prepareToPlay(samplesPerBlock, sampleRate);

	int MaxSamples = MAX_DELAYTIME * sampleRate;
	int CurrentSamples = m_DelayTime.m_Param * sampleRate;
	if (MaxSamples > m_DelayBuffer.GetSize())
		m_DelayBuffer.Resize(MaxSamples, 2, CurrentSamples);
	else
		m_DelayBuffer.SetOffset(CurrentSamples);
}

void TestPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
	m_TransportSource.releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TestPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TestPluginAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
	

	m_NoiseMix.StoreDry(buffer);

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
	Random random = Random::Random();
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* Out = buffer.getWritePointer (channel);
		auto* In = buffer.getReadPointer(channel);

		for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
			Out[sample] = In[sample] * random.nextFloat(); // *m_Mix;
        // ..do something to the data...
    }

	CheckPlayHeadPosition();

	//getSampleRate();
	//buffer.clear();

	MidiBuffer processedMidi;
	int time;
	MidiMessage m;

	for (MidiBuffer::Iterator i(midiMessages); i.getNextEvent(m, time);)
	{
		if (m.isNoteOn())
		{
			uint8 newVel = (uint8)m_NoteOnVel;
			m = MidiMessage::noteOn(m.getChannel(), m.getNoteNumber(), newVel);
		}
		else if (m.isNoteOff())
		{
		}
		else if (m.isAftertouch())
		{
		}
		else if (m.isPitchWheel())
		{
		}

		processedMidi.addEvent(m, time);
	}

	midiMessages.swapWith(processedMidi);

	AudioBuffer<float> FileBuffer = AudioBuffer<float>(buffer.getNumChannels(), buffer.getNumSamples());
	FileBuffer.clear();
	const AudioSourceChannelInfo& BuffertoFill = AudioSourceChannelInfo(FileBuffer);
	BuffertoFill.clearActiveBufferRegion();
	if (m_ReaderSource.get() != nullptr)
	{
		m_TransportSource.getNextAudioBlock(BuffertoFill);
		BufferOperations::Add(buffer, FileBuffer);
	}

	m_NoiseMix.Mix_IntoWet(buffer);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//BufferOperations::Shift(buffer, 20);

	AudioBuffer<float> Delay;
	Delay.makeCopyOf(buffer);

	const float ReversePercent = 0.0f;
	if(random.nextFloat() < ReversePercent)
		m_DelayBuffer.ReadBlockInReverse(Delay);
	else
		m_DelayBuffer.ReadBlock(Delay);

	m_DelayGain.ApplyGain(Delay);
	BufferOperations::Add(buffer, Delay);

	m_DelayBuffer.WriteBlock(buffer);

	
	m_GrainManager.WriteBlock(buffer);
	m_GrainManager.ReadBlock(buffer);

	//BufferOperations::Pan_BasicSin(buffer, 0.0f);

	//BufferOperations::SampleFunction(buffer, [](float x) {return log2(x + 1); });
	//BufferOperations::SampleFunction(buffer, [](float x) {return x < 0.2f ? 0 : 0.5; });

	return;
}

//==============================================================================
bool TestPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* TestPluginAudioProcessor::createEditor()
{
    return new TestPluginAudioProcessorEditor (*this);
}

//==============================================================================
void TestPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TestPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TestPluginAudioProcessor();
}

