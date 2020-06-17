/*
  ==============================================================================

    GainComponent.h
    Created: 24 Mar 2020 11:56:57am
    Author:  lofie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PluginComponent.h"

#define MinusInfinitydB -100.0f

class DecibelSlider : public Slider
{
public:

	String getTextFromValue(double value) override
	{
		return Decibels::toString(value);
	}
	double getValueFromText(const String& text) override
	{
		auto decibelText = text.upToFirstOccurrenceOf("dB", false, false).trim();                     // [1]
		return decibelText.equalsIgnoreCase("-INF") ? MinusInfinitydB : decibelText.getDoubleValue(); // [2]
	}

};

class GainComponent
{
public:

	enum UnitType
	{
		Percent,
		Decibel,
		Float,
	};

	GainComponent(float Gain = 1.0f, UnitType unitType = Percent) : m_Gain(Gain), m_UnitType(unitType) {};
	virtual ~GainComponent()
	{
		if (m_pEditor)
			delete m_pEditor;
	}

	float m_Gain; // Wet/Dry
	UnitType m_UnitType;

	void ApplyGain(AudioBuffer<float>& Dest);

	class Editor : public IPluginComponentEditor, private Slider::Listener
	{
	public:

		Editor(GainComponent* pComponent, AudioProcessorEditor* pParent) : m_pComponent(pComponent), IPluginComponentEditor(pParent) {}

		void Init() override;
		void Resize(int x = -1, int y = -1, int width = -1, int height = -1) override;

		Slider m_GainSlider;

	private:
		GainComponent* m_pComponent;

		void sliderValueChanged(Slider* slider) override;

		void Init_Percent();
		void Init_Decebel();
		void Init_Float();
	};

	void CreateEditor(AudioProcessorEditor* pParent) { m_pEditor = new Editor(this, pParent); }
	Editor* GetEditor() { return m_pEditor; }
	bool HasEditor() { return m_pEditor != nullptr; }

private:

	Editor* m_pEditor = nullptr;

	void ApplyGain_Percent(AudioBuffer<float>& Dest);
	void ApplyGain_Decibel(AudioBuffer<float>& Dest);
	void ApplyGain_Float(AudioBuffer<float>& Dest);

};
