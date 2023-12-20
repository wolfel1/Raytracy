#include "raytracypch.h"
#include "Hitable.h"

#include <glm/gtx/norm.hpp>
#include <cmath>

namespace raytracy::raytracer {
	Sphere::Sphere() : radius(1.0f) {
		center = { 0.0f,0.0f,0.0f };
	}

	Sphere::Sphere(glm::vec3 center, float radius, shared_ptr<Material> material)
		: radius(radius) {
		this->center = center;
		this->material = material;
	}

	bool Sphere::IsHit(const Ray& ray, float min, float max, Hit& hit) const {
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

	Plane::Plane() : size(1.0f) {
		start_point = glm::vec3(0.0f, 0.0f, 0.0f) - glm::vec3(-size, 0.0f, -size);
		Init();
	}

	Plane::Plane(glm::vec3 center, float size, shared_ptr<Material> material)
		: size(size) {
		start_point = center - glm::vec3(size/2, 0.0f, size/2);
		this->material = material;
		Init();
	}

	bool Plane::IsHit(const Ray& ray, float min, float max, Hit& hit) const {
		auto denom = glm::dot(normal, ray.GetDirection());

		if (fabs(denom) == 0) {
			return false;
		}
		auto hit_value = (d - glm::dot(normal, ray.GetOrigin())) / denom;

		if (hit_value < min || hit_value > max) {
			return false;
		}

		auto intersection = ray.PointingAt(hit_value);
		auto hit_point = intersection - start_point;
		auto alpha = dot(w, cross(hit_point, v));
		auto beta = dot(w, cross(u, hit_point));

		if (alpha < 0 || alpha > size*size || beta < 0 || beta > size * size) {
			return false;
		}

		hit.hit_value = hit_value;
		hit.point = intersection;
		hit.SetFaceNormal(ray, normal);
		hit.material = material;
		return true;
	}
	void Plane::Init() {
		u = { size,0.0f,0.0f };
		v = { 0.0f,0.0f, size };
		normal = glm::normalize(glm::cross(u, v));
		w = normal / glm::dot(normal, normal);
		d = glm::dot(normal, start_point);
	}
}