#pragma once

#include <cmath>

#include <Aquila/Aquila.h>
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

static void calculate_height(float* height, int number_of_bars, const Aquila::SpectrumType& spectrum, int freq, float prev_height[])
{
	static double smooth = 0.5;
	//Reading of FFT data starts from index 99 to index index 99 + 127.
	for (int i = 99, j = 0; j < number_of_bars; i+=2, ++j)
	{
		height[j] = (((float)std::abs(spectrum[i]) / freq) + ((float)std::abs(spectrum[i + 1]) / freq)) / 2;
		if (j > 0)
		{
			height[j] = (height[j - 1] * smooth) + (height[j] * (1 - smooth));
		}

		height[j] = prev_height[j] + (float)(height[j] - prev_height[j]) / 3;
	}
}

static void copy_samples(const Aquila::WaveFile & audio, int offset, double * samples, unsigned sample_size)
{
	Aquila::Frame frame(audio, offset, offset + sample_size);

	for(size_t i = 0; i < sample_size; ++i)
		samples[i] = frame.sample(i);
}