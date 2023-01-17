#include "raytracypch.h"
#include "Scene.h"

namespace raytracy {
	bool Scene::HitObjects(const Ray& ray, float min, float max, Hit& hit) const {
		Hit temp;
		bool hit_anything = false;
		auto closest = max;

		for (const auto& object : objects) {
			if (object->HitsObject(ray, min, max, temp)) {
				hit_anything = true;
				if (closest > temp.hit_value) {
					closest = temp.hit_value;
					hit = temp;
				}
			}
		}

		return hit_anything;
	}
}