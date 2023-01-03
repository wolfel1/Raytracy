#pragma once
#include "Hitable.h"
namespace raytracy {
	class Sphere : public Hitable {
	private:
		Point3 center;
		float radius;
		shared_ptr<Material> material;

	public:
		Sphere();
		Sphere(Point3 center, float radius, shared_ptr<Material> material);

		virtual bool HitsObject(const Ray& ray, float min, float max,
								Hit& hit) const override;
	};
}


