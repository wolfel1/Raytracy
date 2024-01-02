#include "raytracypch.h"

#include "Camera.h"

namespace raytracy {
	/*Camera::Camera(const glm::vec3& look_from, const glm::vec3& look_at, const glm::vec3& up, float field_of_view, float aspect_ratio) {
		auto theta = glm::radians(field_of_view);
		auto height = glm::tan(theta / 2);
		auto viewport_height = 2.0f * height;
		auto viewport_width = aspect_ratio * viewport_height;

		w = glm::normalize(look_from - look_at);
		u = glm::normalize(glm::cross(up, w));
		v = glm::cross(w, u);

		origin = look_from;
		horizontal_axis = viewport_width  * u;
		vertical_axis = viewport_height  * v;
		lower_left_corner = origin - horizontal_axis / 2.0f - vertical_axis / 2.0f - w;
	}

	Ray Camera::ShootRay(float s, float t) const {
		return Ray(origin , lower_left_corner + s * horizontal_axis + t * vertical_axis - origin);
	}*/
}