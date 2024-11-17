#pragma once

#include "../api/opengl/OpenGLShader.h"
#include "../api/opengl/OpenGLTexture.h"
namespace raytracy::renderer {

	class Material {
	protected:
		shared_ptr<OpenGLShader> shader;

		public:
		~Material() = default;
		
		virtual void Draw() const = 0;

		virtual void Destroy() = 0;
	};

	class MeshMaterial : public Material {
	private:
		glm::vec4 color;
		shared_ptr<OpenGLUniformBuffer> material_uniform_buffer = nullptr;

	public:
		MeshMaterial(glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f });
		~MeshMaterial() = default;

		

		glm::vec4 GetColor() const {
			return color;
		}

		void SetColor(glm::vec4 const& color) {
			this->color = color;
			material_uniform_buffer->SetVec4("color", color);
		}

		virtual void Draw() const override;

		virtual void Destroy() override {
			material_uniform_buffer = nullptr;
		}
	};

	class WorldMaterial : public Material {
	private:
		shared_ptr<OpenGLTextureCubemap> cube_map;

	public:
		WorldMaterial();
		~WorldMaterial() = default;


		virtual void Draw() const override;

		virtual void Destroy() override {}
	};
}