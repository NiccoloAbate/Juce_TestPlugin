/*
  ==============================================================================

    MixComponent.cpp
    Created: 23 Mar 2020 9:07:09am
    Author:  lofie

  ==============================================================================
*/

#include "MixComponent.h"

void MixComponent::Editor::Init()
{
	m_MixKnob.setSliderStyle(Slider::Rotary);
	m_MixKnob.setRange(0.0, 1.0, 0.01);
	m_MixKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 15);
	m_MixKnob.setPopupDisplayEnabled(true, true, m_pParentEditor);
	m_MixKnob.setTextValueSuffix(" Mix");
	m_MixKnob.setValue(m_pComponent->m_Mix);

	m_pParentEditor->addAndMakeVisible(&m_MixKnob);
	m_MixKnob.addListener(this);
}

void MixComponent::Editor::Resize(int x, int y, int width, int height)
{
	m_MixKnob.setBounds(x, y, width, height);
}

void MixComponent::Editor::sliderValueChanged(Slider* slider)
{
	m_pComponent->m_Mix = m_MixKnob.getValue();
}

void MixComponent::StoreDry(AudioBuffer<float>& Dry)
{
	m_Dry.makeCopyOf(Dry);
}

AudioBuffer<float> MixComponent::Mix(AudioBuffer<float>& Wet)
{
	return AudioBuffer<float>();
}

void MixComponent::Mix_IntoWet(AudioBuffer<float>& Wet)
{
	int WetNumChannels = Wet.getNumChannels();
	for (int channel = 0; channel < WetNumChannels; ++channel)
	{
		float* Wet_Out = Wet.getWritePointer(channel);
		const float* Wet_In = Wet.getReadPointer(channel);
		const float* Dry_In = m_Dry.getReadPointer(channel);

		for (int sample = 0; sample < Wet.getNumSamples(); ++sample)
			Wet_Out[sample] = (Wet_In[sample] * m_Mix) + (Dry_In[sample] * (1.0f - m_Mix));
	}
}

void MixComponent::Mix_IntoDry(AudioBuffer<float>& Wet)
{
	int DryNumChannels = m_Dry.getNumChannels();
	for (int channel = 0; channel < DryNumChannels; ++channel)
	{
		float* Dry_Out = m_Dry.getWritePointer(channel);
		const float* Dry_In = m_Dry.getReadPointer(channel);
		const float* Other_In = Wet.getReadPointer(channel);

		for (int sample = 0; sample < m_Dry.getNumSamples(); ++sample)
			Dry_Out[sample] = (Other_In[sample] * m_Mix) + (Dry_In[sample] * (1.0f - m_Mix));
	}
}
