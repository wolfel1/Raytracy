#pragma once


#include <glm/glm.hpp>

namespace raytracy {


	class PerspectiveCamera {
	private:
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 target = { 0.0f, 0.0f, 0.0f };
		glm::mat4 view_matrix{};
		glm::mat4 projection_matrix{};
		glm::vec3 up = { 0.0f, 1.0f, 0.0f };
		float yaw = -90.0f;
		float pitch = 0.0f;
		float near_clip = 0.1f;
		float far_clip = 100.0f;



	public:
		PerspectiveCamera(float field_of_view, float aspect_ratio);

		void SetPosition(glm::vec3 const& position) {
			this->position = position; RecalculateViewMatrix();
		}

		void SetTarget(glm::vec3 const& target) {
			this->target = target; RecalculateViewMatrix();
		}

		void SetNearClip(float near_clip) {
			this->near_clip = near_clip; RecalculateViewMatrix();
		}

		void SetFarClip(float far_clip) {
			this->far_clip = far_clip; RecalculateViewMatrix();
		}

		void SetPitch(float pitch) {
			this->pitch += pitch;
			
			RecalculateTarget();
		}

		void SetYaw(float yaw) {
			this->yaw += yaw; RecalculateTarget();
		}

		glm::mat4 const& GetProjectionMatrix() const {
			return projection_matrix;
		}
		glm::mat4 const& GetViewMatrix() const {
			return view_matrix;
		}

		glm::vec3 const& GetPosition() const {
			return position;
		}

		void SetProjection(float field_of_view, float aspect_ratio);

	private:
		void RecalculateViewMatrix();
		void RecalculateTarget();
	};
}