#pragma once

#include "helper/Vector3.h"

namespace raytracy {

	class Ray {
	private:
		Point3 origin{};
		glm::vec3 direction{};

	public:
		Ray() = default;
		Ray(const Point3& origin, const glm::vec3 direction);

		Point3 GetOrigin() const { return origin; }
		glm::vec3 GetDirection() const { return direction; }

		Point3 PointingAt(float length) const { return origin + direction * length; }

		glm::vec3 Reflect(const glm::vec3& normal) const;
		glm::vec3 Refract(const glm::vec3& normal, float etai_over_etat) const;
	};
}