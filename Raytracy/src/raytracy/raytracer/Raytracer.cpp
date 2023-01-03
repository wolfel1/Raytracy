#include "raytracypch.h"
#include "Raytracer.h"

namespace raytracy {
	void Raytracer::Init() {
		Random::Init();
	}

	void Raytracer::Shutdown() {
	}
	Color3 Raytracer::ComputeRayColor() {
		return Color3();
	}
}


