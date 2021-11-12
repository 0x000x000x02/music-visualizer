#pragma once

#include <cmath>
#include <cstdio>
#include <algorithm>
#include <kissfft.hh>
#include <SFML/Audio.hpp>

#include <objects.h>

#define PI 3.1415926535

#define SpctType std::complex<int16_t>

static void calculateHeight(double height[], double prevHeight[], const int barCount, const SpctType * spectrum, int sampleFrequency, size_t fftSize)
{
	static const double smooth = 0.5;
	static const size_t gap = fftSize/barCount/2;
	double fMax = 0;

	// Get the max frequency
	for (int i = 1; i < barCount; i++)
		fMax = std::max(height[i-1], height[i]);

	for (int i = 0; i < barCount; i++)
	{
		height[i] = std::pow(height[i] / 12, 0.5) * 1.0/fMax;
		height[i] = double(spectrum[i + 1 + gap].real()) / sampleFrequency;
		if (i > 0)
		{
			height[i] = (height[i - 1] * smooth) + (height[i] * (1 - smooth));
		}

		height[i] = (height[i] < 0) ? height[i] * -1 : height[i];

		height[i] = prevHeight[i] + (double)(height[i] - prevHeight[i]) / 8;
	}
}

static void copySamples(int offset, const sf::Int16 * buffer, size_t bufferSize , SpctType * frame, unsigned frameSize)
{
	for(size_t i = 0; i < frameSize; i++)
	{
		double multiplier = 0.5 * cos(2*PI*i / (frameSize-1));
		frame[i].real((offset+i < bufferSize) ? (multiplier * buffer[offset+i]) : 0);
		frame[i].imag(0);
	}
}
