#pragma once

#include "../api/opengl/OpenGLShader.h"
namespace raytracy::renderer {

	class Material {
	private:
		glm::vec4 color;

		shared_ptr<OpenGLShader> shader;
		static shared_ptr<OpenGLUniformBuffer> material_uniform_buffer;

	public:
		Material(glm::vec4 color = { 0.5f, 0.5f, 0.5f, 1.0f });
		~Material() = default;

		shared_ptr<OpenGLShader> GetShader() const {
			return shader;
		}

		glm::vec4 GetColor() const {
			return color;
		}

		void SetColor(glm::vec4 const& color) {
			this->color = color;
		}

		void Draw() const;

		void Destroy() {
			material_uniform_buffer = nullptr;
		}
	};
}