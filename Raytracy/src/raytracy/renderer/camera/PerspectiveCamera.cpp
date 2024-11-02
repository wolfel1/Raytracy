#include "raytracypch.h"
#include "PerspectiveCamera.h"


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace raytracy {
	PerspectiveCamera::PerspectiveCamera(float field_of_view, float aspect_ratio) {

		projection_matrix = glm::perspective(glm::radians(field_of_view), aspect_ratio, near_clip, far_clip);
		view_matrix = glm::lookAt(position, position + direction, up);
	}

	void PerspectiveCamera::SetPosition(glm::vec3 const& position) {
		this->position = position;
		camera_uniform_buffer->SetVec3("position", position);
		RecalculateViewMatrix();
	}

    void PerspectiveCamera::SetProjection(float field_of_view, float aspect_ratio) {
		projection_matrix = glm::perspective(glm::radians(field_of_view), aspect_ratio, near_clip, far_clip);
    }

    void PerspectiveCamera::RecalculateViewMatrix() {
		view_matrix = glm::lookAt(position, position + direction, up);
	}

	void PerspectiveCamera::RecalculateDirection() {
		if (pitch > 89.0f) {
			pitch = 89.0f;
		} else if (pitch < -89.0f) {
			pitch = -89.0f;
		}

		glm::vec3 direction{};
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		this->direction = glm::normalize(direction);
		RecalculateViewMatrix();
	}
}