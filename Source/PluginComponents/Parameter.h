/*
  ==============================================================================

    Parameter.h
    Created: 5 Apr 2020 2:33:38pm
    Author:  lofie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PluginComponent.h"

template <typename T>
class ParameterComponent
{
public:

	ParameterComponent(T Val = T(NULL))
	{
		m_Param = Val;
	}
	virtual ~ParameterComponent()
	{
		if (m_pEditor)
			delete m_pEditor;
	}

	T m_Param;

	class Editor : public IPluginComponentEditor, private Slider::Listener
	{
	public:

		Editor(ParameterComponent<T>* pComponent, AudioProcessorEditor* pParent) : m_pComponent(pComponent), IPluginComponentEditor(pParent) {}

		void Init() override;
		void Resize(int x = -1, int y = -1, int width = -1, int height = -1) override;

		Slider m_Slider;

		std::function<void(Slider*)> m_OnValueChangedFuntion;

	private:
		ParameterComponent<T>* m_pComponent;

		void sliderValueChanged(Slider* slider) override;
	};

	void CreateEditor(AudioProcessorEditor* pParent) { m_pEditor = new Editor(this, pParent); }
	Editor* GetEditor() { return m_pEditor; }
	bool HasEditor() { return m_pEditor != nullptr; }

private:

	Editor* m_pEditor = nullptr;

};

template<typename T>
void ParameterComponent<T>::Editor::Init()
{
	m_Slider.setSliderStyle(Slider::Rotary);
	m_Slider.setRange(0.0, 1.0, 0.01);
	m_Slider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 15);
	m_Slider.setPopupDisplayEnabled(true, true, m_pParentEditor);
	m_Slider.setTextValueSuffix("");
	m_Slider.setValue(m_pComponent->m_Param);

	m_pParentEditor->addAndMakeVisible(&m_Slider);
	m_Slider.addListener(this);
}

template<typename T>
void ParameterComponent<T>::Editor::Resize(int x, int y, int width, int height)
{
	m_Slider.setBounds(x, y, width, height);
}

template<typename T>
void ParameterComponent<T>::Editor::sliderValueChanged(Slider* slider)
{
	m_pComponent->m_Param = m_Slider.getValue();
	if (m_OnValueChangedFuntion)
		m_OnValueChangedFuntion(slider);
}
