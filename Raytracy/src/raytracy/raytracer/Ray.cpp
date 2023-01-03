#include "raytracypch.h"

#include <glm/gtx/norm.hpp>
#include "Ray.h"

namespace raytracy {
	Ray::Ray(const Point3& origin, const glm::vec3 direction) : origin(origin), direction(direction) {}

	glm::vec3 Ray::Reflect(const glm::vec3& normal) const {
		auto unit_direction = glm::normalize(normal);
		return unit_direction - 2.0f * glm::dot(unit_direction, normal) * normal;
	}

	glm::vec3 Ray::Refract(const glm::vec3& normal, float etai_over_etat) const {
		auto unit_direction = glm::normalize(normal);
		auto cos_theta = fmin(glm::dot(-unit_direction, normal), 1.0f);
		glm::vec3 refracted_out_perp = (unit_direction + cos_theta * normal) * etai_over_etat;
		glm::vec3 refracted_out_parallel = -sqrt(fabs(1.0f - glm::length2(refracted_out_perp))) * normal;
		return refracted_out_perp + refracted_out_parallel;
	}
}