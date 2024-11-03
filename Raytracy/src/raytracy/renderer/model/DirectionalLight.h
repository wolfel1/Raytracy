#pragma once

#include "../api/opengl/OpenGLBuffer.h"

namespace raytracy {

	class DirectionalLight {
	private:
		glm::vec3 color;
		glm::vec3 direction;
		float strength;

		shared_ptr<OpenGLUniformBuffer> light_uniform_buffer = nullptr;

	public:
		DirectionalLight() : color(glm::vec3(1.0f)), direction(glm::vec3(0.0f, -1.0f, 0.0f)), strength(1.0f) {}
		DirectionalLight(glm::vec3 color, glm::vec3 direction, float strength);
		~DirectionalLight() = default;

		glm::vec3 GetColor() const {
			return color;
		}

		glm::vec3 GetDirection() const {
			return direction;
		}

		float GetStrength() const {
			return strength;
		}

		void SetColor(glm::vec3 const& color) {
			this->color = color;
		}

		void SetDirection(glm::vec3 const& direction) {
			this->direction = direction;
		}

		void SetStrength(float strength) {
			this->strength = strength;
		}

		shared_ptr<OpenGLUniformBuffer> GetUniformBuffer() const {
			return light_uniform_buffer;
		}

		void SetUniformBuffer(shared_ptr<OpenGLUniformBuffer> buffer);
	};
}