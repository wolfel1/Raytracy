#include "raytracypch.h"
#include "Sphere.h"

#include <glm/gtx/norm.hpp>

namespace raytracy {
	Sphere::Sphere() : center({ 0.0f,0.0f,0.0f }), radius(1.0f) {}

	Sphere::Sphere(glm::vec3 center, float radius, shared_ptr<Material> material)
		: center(center), radius(radius), material(material) {}

	bool Sphere::HitsObject(const Ray& ray, float min, float max, Hit& hit) const {
		glm::vec3 origin_to_center = ray.GetOrigin() - center;
		auto a = glm::length2(ray.GetDirection());
		auto bias = glm::dot(origin_to_center, ray.GetDirection());
		auto c = glm::length2(origin_to_center) - radius * radius;

		auto discriminant = bias * bias - a * c;
		if (discriminant < 0.0f) {
			return false;
		}

		auto sqrt_discriminant = glm::sqrt(discriminant);
		auto hit_value = (-bias - sqrt_discriminant) / a;
		if (hit_value < min || hit_value > max) {
			hit_value = (-bias + sqrt_discriminant) / a;
			if (hit_value < min || hit_value > max) {
				return false;
			}
		}

		hit.hit_value = hit_value;
		hit.point = ray.PointingAt(hit_value);
		glm::vec3 outward_normal = (hit.point - center) / radius;
		hit.SetFaceNormal(ray, outward_normal);
		hit.material = material;
		return true;
	}
}