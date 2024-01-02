#pragma once

#include "glm/glm.hpp"

namespace raytracy {

	/*class Ray {
	private:
		glm::vec3 origin{};
		glm::vec3 direction{};

	public:
		Ray() = default; 
		Ray(const glm::vec3& origin, const glm::vec3 direction) : origin(origin), direction(glm::normalize(direction)) {}
		~Ray() = default;

		glm::vec3 GetOrigin() const { return origin; }
		glm::vec3 GetDirection() const { return direction; }

		glm::vec3 PointingAt(float length) const { return origin + direction * length; }
	};*/
}