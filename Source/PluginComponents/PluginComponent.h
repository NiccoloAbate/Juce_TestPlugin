/*
  ==============================================================================

    PluginComponent.h
    Created: 23 Mar 2020 9:21:42am
    Author:  lofie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

// To add to if needed
class IPluginComponent
{

};


class IPluginComponentEditor
{
public:
	IPluginComponentEditor(AudioProcessorEditor* pParent) : m_pParentEditor(pParent) {}
	virtual ~IPluginComponentEditor() {}

	AudioProcessorEditor* GetParentEditor() { return m_pParentEditor; }

	virtual void Init() = 0;
	virtual void Resize(int x = -1, int y = -1, int width = -1, int height = -1) = 0;

protected:
	AudioProcessorEditor* m_pParentEditor;
};
