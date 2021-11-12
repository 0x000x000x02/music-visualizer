#pragma once

#include <cmath>
#include <cstdio>
#include <kissfft.hh>
#include <SFML/Audio.hpp>

#include <objects.h>

static void calculateHeight(double height[], double prevHeight[], const int barCount, const std::complex<int16_t> * spectrum, int sampleFrequency, size_t fftSize)
{
	static const double smooth = 0.5;
	static const size_t gap = fftSize/barCount/2;

	for (int i = 0; i < barCount; i++)
	{
		height[i] = double(std::abs(spectrum[i + 1])) / sampleFrequency;
		if (i > 0)
		{
			height[i] = (height[i - 1] * smooth) + (height[i] * (1 - smooth));
		}

		height[i] *= 1000;
		height[i] = prevHeight[i] + (float)(height[i] - prevHeight[i]) / 3;
	}
}

static void copySamples(int offset, const sf::Int16 * buffer, size_t bufferSize , std::complex<int16_t> * frame, unsigned frameSize)
{
	for(size_t i = 0; i < frameSize; i++)
	{
		frame[i].real((offset+i < bufferSize) ? buffer[offset+i] : 0);
		frame[i].imag((offset+i < bufferSize) ? buffer[offset+i] : 0);
	}
}
