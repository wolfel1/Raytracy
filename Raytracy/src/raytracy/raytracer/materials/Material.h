#pragma once

#include "../Ray.h"

namespace raytracy {

	struct Hit;

	class Material {
	public:
		virtual bool Scatter(const Ray& incoming_ray, const Hit& hit,
							 Color3& attenuation, Ray& scattered) const = 0;
	};
}

