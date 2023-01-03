#include "raytracypch.h"
#include "Random.h"

namespace raytracy {
	std::mt19937 Random::generator;
	std::uniform_real_distribution<float> Random::distribution;
}