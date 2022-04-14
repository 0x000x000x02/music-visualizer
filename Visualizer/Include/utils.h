#pragma once

#include <cmath>
#include <cstdio>
#include <algorithm>
#include <kissfft.hh>
#include <SFML/Audio.hpp>

#include <settings.h>
#include <objects.h>

#define PI 3.1415926535

#define SpctType std::complex<int16_t>

static void copySamples(int offset, const sf::Int16* buffer, size_t bufferSize, SpctType* frame)
{
	for (size_t i = 0; i < SAMPLE_SIZE; ++i)
	{
		double multiplier = 0.5 * cos(PI * i * 2.0 / (SAMPLE_SIZE - 1));
		frame[i] = std::complex<int16_t>((offset + i < bufferSize) ? ((int16_t)(multiplier * buffer[offset + i])) : 0, 0);
	}
}

static void calculateHeight(double height[], const SpctType * spectrum, int sampleFrequency, float binSizePerBar)
{
	static const double smooth = 0.7;
	static constexpr float deltaMultiplier = 1.0f/8.0f;

	for (int i = 0; i < NUMBER_OF_BARS; ++i)
	{
		double val = 0;

		for (int j = i; i < NUMBER_OF_BARS; ++j)
		{
			const float freqBin = j * (float)sampleFrequency / FFT_SIZE;

			if (freqBin > (j + 1) * binSizePerBar)
				break;

			if (freqBin > j * binSizePerBar && freqBin < (j + 1) * binSizePerBar)
			{
				val += (double)spectrum[j+1].real() / sampleFrequency;
			}
		}

		if (i > 0)
		{
			val = (height[i - 1] * smooth) + (val * 1 - smooth);
		}

		double deltaVal = (val - height[i]) * deltaMultiplier;
		height[i] += std::max(deltaVal, -height[i] * deltaMultiplier * 2.0f);
	}
}
