/*
  ==============================================================================

    GainComponent.cpp
    Created: 24 Mar 2020 11:56:57am
    Author:  lofie

  ==============================================================================
*/

#define SOURCEFOLDER "D:\Documents\Juce\juce-5.4.7-windows\JUCE\Projects\TestPlugin\Source"

#include "GainComponent.h"
#include "D:\Documents\Juce\juce-5.4.7-windows\JUCE\Projects\TestPlugin\Source\Utils\BufferOperations.h"

void GainComponent::ApplyGain(AudioBuffer<float>& Dest)
{
	switch (m_UnitType)
	{
	case Percent:
		ApplyGain_Percent(Dest);
		break;
	case Decibel:
		ApplyGain_Decibel(Dest);
		break;
	case Float:
		ApplyGain_Float(Dest);
		break;
	default:
		break;
	}
}

void GainComponent::ApplyGain_Percent(AudioBuffer<float>& Dest)
{
	int DestNumChannels = Dest.getNumChannels();
	for (int channel = 0; channel < DestNumChannels; ++channel)
	{
		float* Dest_Out = Dest.getWritePointer(channel);
		const float* Dest_In = Dest.getReadPointer(channel);

		for (int sample = 0; sample < Dest.getNumSamples(); ++sample)
			Dest_Out[sample] = Dest_In[sample] * m_Gain;
	}
}

void GainComponent::ApplyGain_Decibel(AudioBuffer<float>& Dest)
{
	int DestNumChannels = Dest.getNumChannels();
	for (int channel = 0; channel < DestNumChannels; ++channel)
	{
		float* Dest_Out = Dest.getWritePointer(channel);
		const float* Dest_In = Dest.getReadPointer(channel);

		for (int sample = 0; sample < Dest.getNumSamples(); ++sample)
			Dest_Out[sample] = Dest_In[sample] * Decibels::decibelsToGain(m_Gain);
	}
}

void GainComponent::ApplyGain_Float(AudioBuffer<float>& Dest)
{
	AudioBuffer<float> Normalized = BufferOperations::Normalized(Dest);
	int DestNumChannels = Dest.getNumChannels();
	for (int channel = 0; channel < DestNumChannels; ++channel)
	{
		float* Dest_Out = Dest.getWritePointer(channel);
		const float* Dest_In = Dest.getReadPointer(channel);
		const float* Norm_In = Normalized.getReadPointer(channel);

		for (int sample = 0; sample < Dest.getNumSamples(); ++sample)
			Dest_Out[sample] = Dest_In[sample] - Norm_In[sample] * m_Gain;
	}
}

void GainComponent::Editor::Init()
{
	switch (m_pComponent->m_UnitType)
	{
	case Percent:
		Init_Percent();
		break;
	case Decibel:
		Init_Decebel();
		break;
	case Float:
		Init_Float();
		break;
	default:
		break;
	}
}

void GainComponent::Editor::Resize(int x, int y, int width, int height)
{
	m_GainSlider.setBounds(x, y, width, height);
}

void GainComponent::Editor::sliderValueChanged(Slider* slider)
{
	m_pComponent->m_Gain = m_GainSlider.getValue();
}

void GainComponent::Editor::Init_Percent()
{
	m_GainSlider.setSliderStyle(Slider::Rotary);
	m_GainSlider.setRange(0.0, 1.0, 0.01);
	m_GainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 15);
	m_GainSlider.setPopupDisplayEnabled(true, true, m_pParentEditor);
	m_GainSlider.setTextValueSuffix(" %");
	m_GainSlider.setValue(m_pComponent->m_Gain);

	m_pParentEditor->addAndMakeVisible(&m_GainSlider);
	m_GainSlider.addListener(this);
}

void GainComponent::Editor::Init_Decebel()
{
	m_GainSlider.setSliderStyle(Slider::Rotary);
	m_GainSlider.setRange(Decibels::gainToDecibels(0.0f, MinusInfinitydB), Decibels::gainToDecibels(1.0f, MinusInfinitydB), 0.01);
	m_GainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 15);
	m_GainSlider.setPopupDisplayEnabled(true, true, m_pParentEditor);
	m_GainSlider.setTextValueSuffix(" Db");
	m_GainSlider.setValue(m_pComponent->m_Gain);

	m_pParentEditor->addAndMakeVisible(&m_GainSlider);
	m_GainSlider.addListener(this);
}

void GainComponent::Editor::Init_Float()
{
	jassert(false);
}
