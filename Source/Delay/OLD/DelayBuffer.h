/*
  ==============================================================================

    DelayBuffer.h
    Created: 22 Mar 2020 2:10:13pm
    Author:  lofie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

// Basically a moving stack
class DelayBuffer
{
public:

	DelayBuffer(int numFrames)
	{
		SetNumFrames(numFrames);
	}
	DelayBuffer(float Length = 0.0f)
	{
		SetLength(Length);
	}

	//Adds buffer to top of queue, returns buffer on the bottom of queue
	AudioBuffer<float> Next(AudioBuffer<float>& Buffer)
	{
		Push(Buffer);
		if (IsFull())
			return Pop();
		else
			return CreateEmptyBuffer();
	}
	// Queues the buffer
	void Push(AudioBuffer<float>& Buffer)
	{
		//if (Buffer.getMagnitude(0, Buffer.getNumSamples()) > 0)
		AudioBuffer<float> New;
		New.makeCopyOf(Buffer);
		Data.push(New);
	}
	// Removes from the bottom
	AudioBuffer<float> Pop()
	{
		AudioBuffer<float> R = Data.front();
		Data.pop();
		return R;
	}

	bool IsFull()
	{
		if (fLength != -1)
			return (fFrameTime * Data.size() > fLength);
		else if (NumFrames != -1)
			return (Data.size() > NumFrames);
		jassert(false);
		return false;
	}
	bool IsOverFull()
	{
		if (fLength != -1)
			return (fFrameTime * Data.size() > fLength + fFrameTime);
		else if (NumFrames != -1)
			return (Data.size() > NumFrames + 1);
		jassert(false);
		return false;
	}

	int GetNumFrames() { return NumFrames; }
	void SetNumFrames(int numFrames) { NumFrames = numFrames; }
	float GetLength() { return fLength; }
	void SetLength(float Length) { fLength = Length; }

	void SetFrameTime(float Time)
	{
		fFrameTime = Time;
	}

	void Resize()
	{
		int TargetFramesAllowed;
		if (fLength != -1)
			TargetFramesAllowed = floor(fLength / fFrameTime);
		else if (NumFrames != -1)
			TargetFramesAllowed = NumFrames;
		else
			return;
		while (IsFull())
		{
			Data.pop();
		}
	}

private:

	int NumFrames = -1;
	float fLength = -1;
	float fFrameTime = 0.00001;
	std::queue<AudioBuffer<float>> Data;

	// Must have at least one buffer in Data (used if Next function)
	AudioBuffer<float> CreateEmptyBuffer()
	{
		AudioBuffer<float> R = AudioBuffer<float>(Data.front().getNumChannels(), Data.front().getNumSamples());
		R.clear();
		return R;
	}

};
