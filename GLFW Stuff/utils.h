#pragma once

#include <cmath>
#include <cstdio>
#include <aquila/aquila.h>
#include <SFML/Audio.hpp>

#include "objects.h"

static void calculate_height(float height[], int number_of_bars, const Aquila::SpectrumType& spectrum, int freq, float prev_height[], size_t fft_size)
{
	static const double smooth = 0.5;
	static const size_t gap = fft_size/number_of_bars/2;

	for (int i = 0; i < number_of_bars; i++)
	{
		height[i] = ((float)std::abs(spectrum[i+8])) / freq;
		if (i > 0)
		{
			height[i] = (height[i - 1] * smooth) + (height[i] * (1 - smooth));
		}

		height[i] = prev_height[i] + (float)(height[i] - prev_height[i]) / 10;
	}
}

static void copy_samples(const Aquila::WaveFile & audio, int offset, double samples[], unsigned sample_size)
{
	Aquila::Frame frame(audio, offset, offset + sample_size);
	static const Aquila::HammingWindow hwindow(sample_size);

	for(size_t i = 0; i < sample_size; ++i) {
		samples[i] = frame.sample(i);
	}
}
