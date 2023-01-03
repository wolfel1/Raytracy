#pragma once

#include <fstream>

#include "Vector3.h"

inline float Clamp(float x, float min, float max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}
void WriteColor(std::ofstream& out, Color3 pixel_color, int samples_per_pixel) {
	auto red = pixel_color.r;
	auto green = pixel_color.g;
	auto blue = pixel_color.b;

	auto scale = 1.0f / samples_per_pixel;
	red = sqrt(scale * red);
	green = sqrt(scale * green);
	blue = sqrt(scale * blue);

	out << static_cast<int>(256.0f * Clamp(red, 0.0f, 0.999f)) << ' '
		<< static_cast<int>(256.0f * Clamp(green, 0.0f, 0.999f)) << ' '
		<< static_cast<int>(256.0f * Clamp(blue, 0.0f, 0.999f)) << '\n';
}