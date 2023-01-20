#pragma once
#include "Hitable.h"

namespace raytracy {
	class Sphere : public Hitable {
	private:
		glm::vec3 center;
		float radius;
		shared_ptr<Material> material;

	public:
		Sphere();
		Sphere(glm::vec3 center, float radius, shared_ptr<Material> material);

		virtual bool HitsObject(const Ray& ray, float min, float max,
								Hit& hit) const override;
	};
}


