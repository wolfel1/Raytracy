#pragma once

#include "../../core/Core.h"
#include "../Ray.h"
#include "../helper/Hit.h"
#include "../materials/Material.h"

namespace raytracy {

	class Hitable {
	protected:
		glm::vec3 center;
		shared_ptr<Material> material;

	public:
		virtual bool IsHit(const Ray& ray, float min, float max,
								Hit& hit) const = 0;
	};

	class Sphere : public Hitable {
	private:
		float radius;

	public:
		Sphere();
		Sphere(glm::vec3 center, float radius, shared_ptr<Material> material);
		~Sphere() = default;

		virtual bool IsHit(const Ray& ray, float min, float max,
								Hit& hit) const override;
	};
}
