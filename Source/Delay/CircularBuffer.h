/*
  ==============================================================================

    CircularBuffer.h
    Created: 27 May 2020 3:43:46pm
    Author:  lofie

  ==============================================================================
*/

#pragma once



template<typename T, unsigned int size>
class CircularBuffer
{
public:

	//CircularBuffer(unsigned int Offset = size) : readOffset(Offset) {}
	CircularBuffer(unsigned int Offset = size, T DefaultValue = (T)0) : readOffset(Offset) { Clear(DefaultValue); }

	T Process(T In)
	{
		int GG = GetReadIndex();
		T Out = data[GetReadIndex()];
		data[write] = In;
		write = (write + 1) >= size ? 0 : write + 1;
		return Out;
	}
	void Process(T In, T& Out)
	{
		Out = data[GetReadIndex()];
		data[write] = In;
		write = (write + 1) >= size ? 0 : write + 1;
	}
	void Process_Ref(T &InOut)
	{
		T In = InOut;
		Out = data[GetReadIndex()];
		data[write] = In;
		write = (write + 1) >= size ? 0 : write + 1;
	}

	T Read()
	{
		Out = data[GetReadIndex()];
		--readOffset;
	}

	void Write(T In)
	{
		data[write] = In;
		++readOffset;
		write = (write + 1) >= size ? 0 : write + 1;
	}

	// return (write - readOffset) if In bounds; else returns (write - readOffset) + size
	int GetReadIndex()
	{
		return ((write - readOffset) < 0 ? size + (write - readOffset) : (write - readOffset));
	}

	void Clear(T DefaultValue = (T)0)
	{
		for (int i = 0; i < size; ++i)
		{
			data[i] = DefaultValue;
		}
	}

	T data[size];
	int write = 0;
	int readOffset;

};

