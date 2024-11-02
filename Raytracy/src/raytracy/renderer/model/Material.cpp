#include "raytracypch.h"
#include "Material.h"

#include "../api/Shader.h"
#include "../ViewportScene.h"

namespace raytracy::renderer {
	shared_ptr<OpenGLUniformBuffer> MeshMaterial::material_uniform_buffer = nullptr;

	MeshMaterial::MeshMaterial(glm::vec4 color) : color(color) {
		shader = ShaderLibrary::Get().Load("default");
		RTY_ASSERT(shader, "Could not create a shader program!");

		if (!material_uniform_buffer) {

			UniformBlock block("Material", {
				"color",
			});

			auto layout = shader->GetUniformBufferLayout(block);

			material_uniform_buffer = OpenGLUniformBuffer::Create("Material", layout);
			shader->AddUniformBuffer(material_uniform_buffer);
		}
	}

	void MeshMaterial::Draw() const {
		material_uniform_buffer->SetVec4("color", color);
		shader->Bind();
		shader->SetInt("skybox", 1);
	}

	WorldMaterial::WorldMaterial() {
		shader = ShaderLibrary::Get().Load("world");

		cube_map = OpenGLTextureCubemap::Create({{
			"skybox/right.jpg",
			"skybox/left.jpg",
			"skybox/top.jpg",
			"skybox/bottom.jpg",
			"skybox/front.jpg",
			"skybox/back.jpg"
		} });
		cube_map->Bind(1);
	}

	void WorldMaterial::Draw() const {

		shader->Bind();
		shader->SetInt("skybox", 1);
	}
}