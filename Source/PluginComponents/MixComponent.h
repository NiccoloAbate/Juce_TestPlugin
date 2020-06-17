/*
  ==============================================================================

    MixComponent.h
    Created: 23 Mar 2020 9:07:09am
    Author:  lofie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PluginComponent.h"

class MixComponent
{
public:

	MixComponent(float Mix = 1.0) : m_Mix(Mix) {};
	virtual ~MixComponent()
	{
		if (m_pEditor)
			delete m_pEditor;
	}

	float m_Mix; // Wet/Dry

	void StoreDry(AudioBuffer<float>& Dry);
	AudioBuffer<float>& GetDry() { return m_Dry; }
	AudioBuffer<float> Mix(AudioBuffer<float>& Wet);
	void Mix_IntoWet(AudioBuffer<float>& Wet);
	void Mix_IntoDry(AudioBuffer<float>& Wet);

	class Editor : public IPluginComponentEditor, private Slider::Listener
	{
	public:
		
		Editor(MixComponent* pComponent, AudioProcessorEditor* pParent) : m_pComponent(pComponent), IPluginComponentEditor(pParent) {}

		void Init() override;
		void Resize(int x = -1, int y = -1, int width = -1, int height = -1) override;

		Slider m_MixKnob;
		
	private:
		MixComponent* m_pComponent;
		
		void sliderValueChanged(Slider* slider) override;
	};

	void CreateEditor(AudioProcessorEditor* pParent) { m_pEditor = new Editor(this, pParent); }
	Editor* GetEditor() { return m_pEditor; }
	bool HasEditor() { return m_pEditor != nullptr; }

private:

	Editor* m_pEditor = nullptr;

	AudioBuffer<float> m_Dry;

};
