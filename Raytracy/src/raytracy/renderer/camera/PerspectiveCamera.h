#pragma once


#include <glm/glm.hpp>
#include "../api/opengl/OpenGLBuffer.h"

namespace raytracy {


	class PerspectiveCamera {
	private:
		glm::vec3 position = { 0.0f, 0.0f, -5.0f };
		glm::vec3 direction = { 0.0f, 0.0f, 1.0f };
		glm::mat4 view_matrix{};
		glm::mat4 projection_matrix{};
		glm::vec3 up = { 0.0f, 1.0f, 0.0f };
		float yaw = -90.0f;
		float pitch = 0.0f;
		float near_clip = 0.1f;
		float far_clip = 100.0f;

		shared_ptr<OpenGLUniformBuffer> camera_uniform_buffer;

	public:
		PerspectiveCamera(float field_of_view, float aspect_ratio);
		~PerspectiveCamera() = default;

		void SetPosition(glm::vec3 const& position);

		void SetDirection(glm::vec3 const& direction) {
			this->direction = direction; RecalculateViewMatrix();
		}

		void SetNearClip(float near_clip) {
			this->near_clip = near_clip; RecalculateViewMatrix();
		}

		void SetFarClip(float far_clip) {
			this->far_clip = far_clip; RecalculateViewMatrix();
		}

		void SetPitch(float pitch) {
			this->pitch += pitch;
			
			RecalculateDirection();
		}

		void SetYaw(float yaw) {
			this->yaw += yaw; RecalculateDirection();
		}

		float GetYaw() const {
			return yaw;
		}

		float GetPitch() const {
			return pitch;
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
		
		glm::vec3 const& GetDirection() const {
			return direction;
		}

		glm::vec3 const& GetUp() const {
			return up;
		}

		void SetCameraUniformBuffer(shared_ptr<OpenGLUniformBuffer> const buffer) {
			camera_uniform_buffer = buffer;
			camera_uniform_buffer->SetVec3("position", position);
		}

		shared_ptr<OpenGLUniformBuffer> GetCameraUniformBuffer() const {
			return camera_uniform_buffer;
		}

		void SetProjection(float field_of_view, float aspect_ratio);

	private:
		void RecalculateViewMatrix();
		void RecalculateDirection();
	};
}