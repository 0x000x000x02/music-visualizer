#pragma once

#include <cmath>

#include <aquila/aquila.h>
#include <SFML/Audio.hpp>

#include "objects.h"

static void set_color(int val, float * r, float * g, float * b)
{
	if (val < 10)
	{
		*r = 0.f;
		*g = 1.0f - ((float)val / 10);
		*b = (float)val / 10;
	}
	else if (val >= 10 && val < 20)
	{
		*r = 0.f;
		*g = (float)(val - 10) / 10;
		*b = 1.0f - ((float)(val - 10) / 10);
	}
}

static void calculate_height(float* height, int number_of_bars, const Aquila::SpectrumType& spectrum, int freq, float prev_height[], size_t fft_size)
{
	static double smooth = 0.5;
	static const size_t gap = fft_size/number_of_bars/2;

	//Reading of FFT data starts from index 99 to index index 99 + number of bars.
	for (int i = 0, j = 2; i < number_of_bars && j < fft_size; i++, j+=gap)
	{
		height[i] = ((float)std::abs(spectrum[j])) / freq;
		if (i > 0)
		{
			height[i] = (height[i - 1] * smooth) + (height[i] * (1 - smooth));
		}

		height[i] = prev_height[i] + (float)(height[i] - prev_height[i]) / 3;
	}
}

static void copy_samples(const Aquila::WaveFile & audio, int offset, double * samples, unsigned sample_size)
{
	Aquila::Frame frame(audio, offset, offset + sample_size);

	for(size_t i = 0; i < sample_size; ++i)
		samples[i] = frame.sample(i);
}