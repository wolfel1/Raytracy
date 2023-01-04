#pragma once

#include "raytracy/core/Core.h"
#include "Vector3.h"
#include "../Ray.h"

namespace raytracy {

	class Material;

	struct Hit {
		Point3 point{};
		glm::vec3 normal{};
		shared_ptr<Material> material;
		float hit_value{};
		bool front_face{};

		inline void SetFaceNormal(const Ray& ray, const glm::vec3& outward_normal) {
			front_face = glm::dot(ray.GetDirection(), outward_normal) < 0;
			normal = front_face ? outward_normal : -outward_normal;
		}
	};
}