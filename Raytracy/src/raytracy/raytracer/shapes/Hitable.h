#pragma once

#include "../../core/Core.h"
#include "../Ray.h"
#include "../helper/Hit.h"
#include "../materials/Material.h"

namespace raytracy {

	class Hitable {
	public:
		virtual bool HitsObject(const Ray& ray, float min, float max,
								Hit& hit) const = 0;
	};
}
