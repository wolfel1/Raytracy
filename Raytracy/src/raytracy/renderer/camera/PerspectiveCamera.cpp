#include "raytracypch.h"
#include "PerspectiveCamera.h"


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace raytracy {
	PerspectiveCamera::PerspectiveCamera(float field_of_view, float aspect_ratio) {

		projection_matrix = glm::perspective(glm::radians(field_of_view), aspect_ratio, near_clip, far_clip);
		view_matrix = glm::lookAt(position, target, up);
	}


    void PerspectiveCamera::SetProjection(float field_of_view, float aspect_ratio) {
		projection_matrix = glm::perspective(glm::radians(field_of_view), aspect_ratio, near_clip, far_clip);
    }

    void PerspectiveCamera::RecalculateViewMatrix() {
		view_matrix = glm::lookAt(position, target, up);
	}

	void PerspectiveCamera::RecalculateTarget() {
		if (pitch > 89.0f) {
			pitch = 89.0f;
		} else if (pitch < -89.0f) {
			pitch = -89.0f;
		}

		glm::vec3 direction{};
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		target = position + direction;
		RecalculateViewMatrix();
	}
}