#include "raytracypch.h"
#include "Scene.h"

namespace raytracy {
	bool Scene::HitObjects(const Ray& ray, float min, float max, Hit& hit) const {
		Hit temp;
		bool hit_anything = false;
		auto closest = max;

		for (const auto& object : objects) {
			if (object->IsHit(ray, min, closest, temp)) {
				closest = temp.hit_value;
				hit_anything = true;
				hit = temp;

			}
		}

		return hit_anything;
	}
}