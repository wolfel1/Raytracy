#pragma once

#include "helper/Vector3.h"

namespace raytracy {

	class Ray {
	private:
		Point3 origin{};
		glm::vec3 direction{};

	public:
		Ray() = default; 
		Ray(const Point3& origin, const glm::vec3 direction) : origin(origin), direction(glm::normalize(direction)) {}

		Point3 GetOrigin() const { return origin; }
		glm::vec3 GetDirection() const { return direction; }

		Point3 PointingAt(float length) const { return origin + direction * length; }
	};
}