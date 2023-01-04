#pragma once

#include "helper/Vector3.h"
#include "Ray.h"

namespace raytracy {
	class Camera {
	private:
		Point3 origin;
		Point3 lower_left_corner;
		glm::vec3 horizontal_axis;
		glm::vec3 vertical_axis;
		glm::vec3 u, v, w;
		float lens_radius;
	public:
		Camera(const Point3& look_from, const Point3& look_at, const glm::vec3& up, float field_of_view, float aspect_ratio, float apperture, float focus);

		Ray ShootRay(float u, float v) const;
	};
}
