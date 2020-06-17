/*
  ==============================================================================

    BufferOperations.h
    Created: 22 Mar 2020 1:30:19pm
    Author:  lofie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class BufferOperations
{
public:

	// Copies data from Other and writes it in Dest
	static void Copy(AudioBuffer<float>& Dest, AudioBuffer<float>& Other)
	{
		int DestNumChannels = Dest.getNumChannels();
		for (int channel = 0; channel < DestNumChannels; ++channel)
		{
			float* Dest_Out = Dest.getWritePointer(channel);
			const float* Other_In = Other.getReadPointer(channel);

			for (int sample = 0; sample < Dest.getNumSamples(); ++sample)
				Dest_Out[sample] = Other_In[sample];
		}
	}

	// Add data from Other to data from Dest and writes it to Dest
	static void Add(AudioBuffer<float>& Dest, AudioBuffer<float>& Other)
	{
		int DestNumChannels = Dest.getNumChannels();
		for (int channel = 0; channel < DestNumChannels; ++channel)
		{
			float* Dest_Out = Dest.getWritePointer(channel);
			const float* Dest_In = Dest.getReadPointer(channel);
			const float* Other_In = Other.getReadPointer(channel);

			for (int sample = 0; sample < Dest.getNumSamples() && sample < Other.getNumSamples(); ++sample)
				Dest_Out[sample] = Dest_In[sample] + Other_In[sample];
		}
	}

	//Normalizes the buffer
	static void Normalize(AudioBuffer<float>& Dest)
	{
		int NumChannels = Dest.getNumChannels();
		float Multiplier = 1 / Dest.getMagnitude(0, Dest.getNumSamples());
		for (int channel = 0; channel < NumChannels; ++channel)
		{
			float* Out = Dest.getWritePointer(channel);
			const float* In = Dest.getReadPointer(channel);

			for (int sample = 0; sample < Dest.getNumSamples(); ++sample)
				Out[sample] = In[sample] * Multiplier;
		}
	}
	// Returns a new normalized copy of the buffer
	static AudioBuffer<float> Normalized(AudioBuffer<float>& buffer)
	{
		AudioBuffer<float> New;
		New.makeCopyOf(buffer);
		Normalize(New);
		return New;
	}

#define LEFTCHANNEL 0
#define RIGHTCHANNEL 1

#define Pi 3.14159265359

	// -1.0 is left, 0.0 is center, 1.0 is right
	static void Pan_BasicSin(AudioBuffer<float>& Dest, float Pan)
	{
		Pan = jlimit(-1.0f, 1.0f, Pan);
		float Angle = ((Pan + 1.0) / 2.0 * (Pi / 2.0));
		int NumChannels = Dest.getNumChannels();
		for (int channel = 0; channel < NumChannels; ++channel)
		{
			float* Out = Dest.getWritePointer(channel);
			const float* In = Dest.getReadPointer(channel);

			for (int sample = 0; sample < Dest.getNumSamples(); ++sample)
			{
				if (channel % 2 == LEFTCHANNEL)
					Out[sample] = In[sample] * cos(Angle);
				else
					Out[sample] = In[sample] * sin(Angle);
			}
		}
	}

	static void SampleFunction(AudioBuffer<float>& Dest, std::function<float(float)> Func)
	{
		int NumChannels = Dest.getNumChannels();
		for (int channel = 0; channel < NumChannels; ++channel)
		{
			float* Out = Dest.getWritePointer(channel);
			const float* In = Dest.getReadPointer(channel);

			for (int sample = 0; sample < Dest.getNumSamples(); ++sample)
			{
				Out[sample] = Func(In[sample]);
			}
		}
	}

	static void Shift(AudioBuffer<float>& Buffer, int ShiftSamples, bool bWrap = false)
	{
		int NumChannels = Buffer.getNumChannels();
		for (int channel = 0; channel < NumChannels; ++channel)
		{
			float* Out = Buffer.getWritePointer(channel);
			const float* In = Buffer.getReadPointer(channel);

			if (ShiftSamples < 0)
			{
				for (int sample = 0; sample < Buffer.getNumSamples(); ++sample)
				{
					int SourceSampleIndex = sample - ShiftSamples;
					if (SourceSampleIndex >= Buffer.getNumSamples())
					{
						if (bWrap)
						{

						}
						else
							Out[sample] = 0;
					}
					else
						Out[sample] = In[SourceSampleIndex];
				}
			}
			else
			{
				for (int sample = Buffer.getNumSamples() - 1; sample >= 0; --sample)
				{
					int SourceSampleIndex = sample - ShiftSamples;
					if (SourceSampleIndex < 0)
					{
						if (bWrap)
						{

						}
						else
							Out[sample] = 0;
					}
					else
						Out[sample] = In[SourceSampleIndex];
				}
			}
		}
	}

private:
	static void Operate(AudioBuffer<float>& Dest, AudioBuffer<float>& Other, std::function<float(float, float)> Op)
	{
	}
};

