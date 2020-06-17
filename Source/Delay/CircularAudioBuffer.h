/*
  ==============================================================================

    CircularAudioBuffer.h
    Created: 2 Jun 2020 8:39:58pm
    Author:  lofie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

template<typename T>
class CircularAudioBuffer
{
public:

#define INCREMENT(x) x = (x + 1) >= GetSize() ? 0 : x + 1
#define DECREMENT(x) x = (x - 1) < 0 ? GetSize() - 1 : x - 1
#define INCREMENT_AMOUNT(x, amount) x = (x + amount) >= GetSize() ? (x + amount) - GetSize() : (x + amount)
#define DECREMENT_AMOUNT(x, amount) x = (x - amount) < 0 ? (x - amount) + GetSize() : (x - amount)

	CircularAudioBuffer() = default;
	//CircularBuffer(unsigned int Offset = size) : readOffset(Offset) {}
	CircularAudioBuffer(unsigned int Samples, unsigned int Channels, unsigned int Offset, T DefaultValue = (T)0)
	{
		//read = (write - Offset) < 0 ? Samples + (write - Offset) : (write - Offset));
		read = Samples - Offset; // The above simplifies to this
		Resize(Samples, Channels);  
		Clear(DefaultValue);
	}

	void ProcessBlock(AudioBuffer<T>& Block)
	{
		int LoopWrite;
		int LoopRead;

		int NumChannels = buffer.getNumChannels();
		for (int channel = 0; channel < NumChannels; ++channel)
		{
			float* Block_Write = Block.getWritePointer(channel);
			const float* Block_Read = Block.getReadPointer(channel);
			float* buffer_Write = buffer.getWritePointer(channel);
			const float* buffer_Read = buffer.getReadPointer(channel);

			LoopWrite = write;
			LoopRead = read;

			for (int sample = 0; sample < Block.getNumSamples(); ++sample)
			{
				T Buf = Block_Read[sample];
				Block_Write[sample] = buffer_Read[LoopRead];
				buffer_Write[LoopWrite] = Buf;

				INCREMENT(LoopWrite);
				INCREMENT(LoopRead);
			}
		}

		write = LoopWrite;
		read = LoopRead;
	}

	void WriteBlock(AudioBuffer<T>& Block)
	{
		int LoopWrite;

		int NumChannels = buffer.getNumChannels();
		for (int channel = 0; channel < NumChannels; ++channel)
		{
			float* Write = buffer.getWritePointer(channel);
			const float* Read = Block.getReadPointer(channel);

			LoopWrite = write;

			for (int sample = 0; sample < Block.getNumSamples(); ++sample)
			{
				Write[LoopWrite] = Read[sample];
				INCREMENT(LoopWrite);
			}
		}

		write = LoopWrite;
	}

	void ReadBlock(AudioBuffer<T>& Block)
	{
		int LoopRead;

		int NumChannels = buffer.getNumChannels();
		for (int channel = 0; channel < NumChannels; ++channel)
		{
			float* Write = Block.getWritePointer(channel);
			const float* Read = buffer.getReadPointer(channel);

			LoopRead = read;

			for (int sample = 0; sample < Block.getNumSamples(); ++sample)
			{
				Write[sample] = Read[LoopRead];
				INCREMENT(LoopRead);
			}
		}

		read = LoopRead;
	}
	// Should really only be used if reading a big block, otherwise just kind of distorts the sound a bit
	void ReadBlockInReverse(AudioBuffer<T>& Block)
	{
		INCREMENT_AMOUNT(read, Block.getNumSamples());
		int LoopRead;

		int NumChannels = buffer.getNumChannels();
		for (int channel = 0; channel < NumChannels; ++channel)
		{
			float* Write = Block.getWritePointer(channel);
			const float* Read = buffer.getReadPointer(channel);

			LoopRead = read;

			for (int sample = 0; sample < Block.getNumSamples(); ++sample)
			{
				Write[sample] = Read[LoopRead];
				DECREMENT(LoopRead);
			}
		}
	}


	void WriteSample(std::vector<float>& Sample)
	{
		int NumChannels = buffer.getNumChannels();
		for (int channel = 0; channel < NumChannels; ++channel)
		{
			float* Write = buffer.getWritePointer(channel);
			Write[write] = Sample[channel];
		}
		INCREMENT(write);
	}
	void ReadSample(std::vector<float>& Out)
	{
		int NumChannels = buffer.getNumChannels();
		for (int channel = 0; channel < NumChannels; ++channel)
		{
			const float* Read = buffer.getReadPointer(channel);
			Out[channel] = Read[read];
		}
		INCREMENT(read);
	}



	void Clear(T DefaultValue = (T)0)
	{
		int NumChannels = buffer.getNumChannels();
		for (int channel = 0; channel < NumChannels; ++channel)
		{
			float* Out = buffer.getWritePointer(channel);

			for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
				Out[sample] = DefaultValue;
		}
	}

	void Resize(unsigned int Samples, unsigned int Channels)
	{
		buffer.setSize(Channels, Samples, true);
	}

	void Resize(unsigned int Samples, unsigned int Channels, int Offset)
	{
		buffer.setSize(Channels, Samples, true);

		int R = write - Offset;
		read = R < 0 ? R + GetSize() : R;
	}
	
	int GetSize()
	{
		return buffer.getNumSamples();
	}

	void SetOffset(unsigned int Offset)
	{
		if (Offset > GetSize())
			Resize(Offset, buffer.getNumChannels());

		int R = write - Offset;
		read = R < 0 ? R + GetSize() : R;
	}

	//void IncrementWrite()
	//{
		//write = (write + 1) >= GetSize() ? 0 : write + 1;
	//}
	

	// return (write - readOffset) if In bounds; else returns (write - readOffset) + size

	AudioBuffer<T> buffer;
	int write = 0;
	int read;

};



