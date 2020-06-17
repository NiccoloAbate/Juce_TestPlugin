/*
  ==============================================================================

    DelayBuffer.h
    Created: 22 Mar 2020 2:10:13pm
    Author:  lofie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "CircularAudioBuffer.h"

template<typename T>
class DelayBuffer : public CircularAudioBuffer<T>
{
public:

	DelayBuffer() = default;
	DelayBuffer(int Samples, int Channels, int Offset) : CircularAudioBuffer<float>(Samples, Channels, Offset) {}

	
};
