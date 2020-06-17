/*
  ==============================================================================

    GrainManager.h
    Created: 16 Jun 2020 12:15:08pm
    Author:  lofie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "CircularAudioBuffer.h"


template <typename T>
class GrainManager
{
public:

#define INCREMENT_GRAIN(x) x = (x + 1) >= m_Grains.size() ? 0 : x + 1

	GrainManager() = default;
	GrainManager(unsigned int Samples, unsigned int Channels, int NumGrains)
	{
		Resize(Samples, Channels, NumGrains);
		
		m_WriteGrain = 1;
		SetReversePercentage(0.77f);
	}

	void Resize(unsigned int Samples, unsigned int Channels, unsigned int NumGrains)
	{
		m_Grains.resize(NumGrains);
		for (int i = 0; i < m_Grains.size(); ++i)
			m_Grains[i].Resize(Samples, Channels, 0);
	}



	void WriteBlock(AudioBuffer<T>& Block)
	{
		std::vector<const float*> ReadPointers = std::vector<const float*>(Block.getNumChannels());
		for (int channel = 0; channel < Block.getNumChannels(); ++channel)
			ReadPointers[channel] = Block.getReadPointer(channel);							// Compiles a list of read pointers from all channels

		std::vector<float> Sample = std::vector<float>(Block.getNumChannels());
		for (int sample = 0; sample < Block.getNumSamples(); ++sample)
		{
			for (int channel = 0; channel < Block.getNumChannels(); ++channel)
				Sample[channel] = ReadPointers[channel][sample];

			m_Grains[m_WriteGrain].WriteSample(Sample);

			if (m_Grains[m_WriteGrain].write == 0)	// if the circularaudiobuffer has wrapped around
				NextWriteGrain();					// increment / sets next write grain index
		}
	}

	void ReadBlock(AudioBuffer<T>& Block)
	{
		std::vector<float*> WritePointers = std::vector<float*>(Block.getNumChannels());
		for (int channel = 0; channel < Block.getNumChannels(); ++channel)
			WritePointers[channel] = Block.getWritePointer(channel);						// Compiles a list of write pointers from all channels

		std::vector<float> Sample = std::vector<float>(Block.getNumChannels());
		for (int sample = 0; sample < Block.getNumSamples(); ++sample)
		{
			m_Grains[m_ReadGrain].ReadSample(Sample);

			for (int channel = 0; channel < Block.getNumChannels(); ++channel)
				WritePointers[channel][sample] = Sample[channel];

			if(m_bReadReverse)
				m_Grains[m_ReadGrain].read = (m_Grains[m_ReadGrain].read - 2) < 0 ? (m_Grains[m_ReadGrain].read - 2) + m_Grains[m_ReadGrain].GetSize() : (m_Grains[m_ReadGrain].read - 2);

			if (m_Grains[m_ReadGrain].read == 0)	// if the circularaudiobuffer has wrapped around
				NextReadGrain();					// increments / gives next read grain index
		}
	}
	/*
	void ReadBlock_Forward(AudioBuffer<T>& Block)
	{
		std::vector<float*> WritePointers = std::vector<float*>(Block.getNumChannels());
		for (int channel = 0; channel < Block.getNumChannels(); ++channel)
			WritePointers[channel] = Block.getWritePointer(channel);						// Compiles a list of write pointers from all channels

		std::vector<float> Sample = std::vector<float>(Block.getNumChannels());
		for (int sample = 0; sample < Block.getNumSamples(); ++sample)
		{
			m_Grains[m_ReadGrain].ReadSample(Sample);

			for (int channel = 0; channel < Block.getNumChannels(); ++channel)
				WritePointers[channel][sample] = Sample[channel];

			if (m_Grains[m_ReadGrain].read == 0)	// if the circularaudiobuffer has wrapped around
				NextReadGrain();
		}
	}
	void ReadBlock_Reverse(AudioBuffer<float>& Block)
	{
		std::vector<float*> WritePointers = std::vector<float*>(Block.getNumChannels());
		for (int channel = 0; channel < Block.getNumChannels(); ++channel)
			WritePointers[channel] = Block.getWritePointer(channel);						// Compiles a list of write pointers from all channels

		std::vector<float> Sample = std::vector<float>(Block.getNumChannels());
		for (int sample = 0; sample < Block.getNumSamples(); ++sample)
		{
			m_Grains[m_ReadGrain].ReadSample(Sample);

			for (int channel = 0; channel < Block.getNumChannels(); ++channel)
				WritePointers[channel][sample] = Sample[channel];

			m_Grains[m_ReadGrain].read = (m_Grains[m_ReadGrain].read - 2) < 0 ? (m_Grains[m_ReadGrain].read - 2) + m_Grains[m_ReadGrain].GetSize() : (m_Grains[m_ReadGrain].read - 2);

			if (m_Grains[m_ReadGrain].read == 0)	// if the circularaudiobuffer has wrapped around
				NextReadGrain();					// increments / gives next read grain index
		}
	}
	*/



	void NextWriteGrain()
	{
		INCREMENT_GRAIN(m_WriteGrain);
	}
	void NextReadGrain()
	{
		INCREMENT_GRAIN(m_ReadGrain);
		SetReadDirection();
	}



	void SetReadDirection()
	{
		if (m_ReversePercentage < 0)
		{
			m_bReadReverse = !m_bReadReverse;
			return;
		}
		if (Random::Random().nextFloat() < m_ReversePercentage)
			m_bReadReverse = true;
		else
			m_bReadReverse = false;
	}
	void SetReversePercentage(float Percent) { m_ReversePercentage = Percent; }



	std::vector<CircularAudioBuffer<T>> m_Grains;
	int m_WriteGrain = 0;
	int m_ReadGrain = 0;
	bool m_bReadReverse = false;
	float m_ReversePercentage = 0.0f;	// percentage; or if -1: Alternating

	int m_GrainSize;
	int m_NumGrains;

};

