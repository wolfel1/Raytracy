#pragma once

#include <glm/glm.hpp>
#include "Ray.h"

namespace raytracy {
	class Camera {
	private:
		glm::vec3 origin;
		glm::vec3 lower_left_corner;
		glm::vec3 horizontal_axis;
		glm::vec3 vertical_axis;
		glm::vec3 u, v, w;
	public:
		Camera(const glm::vec3& look_from, const glm::vec3& look_at, const glm::vec3& up, float field_of_view, float aspect_ratio);
		~Camera() = default;

		Ray ShootRay(float s, float t) const;
	};
}
