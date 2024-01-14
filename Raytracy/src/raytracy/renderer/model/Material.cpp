#include "raytracypch.h"
#include "Material.h"

#include "../api/Shader.h"

namespace raytracy::renderer {
	shared_ptr<OpenGLUniformBuffer> MeshMaterial::material_uniform_buffer = nullptr;

	MeshMaterial::MeshMaterial(glm::vec4 color) : color(color) {
		shader = ShaderLibrary::Get().Load("Basic");
		RTY_ASSERT(shader, "Could not create a shader program!");

		if (!material_uniform_buffer) {

			UniformBlock block("Material", {
				"color",
			});

			auto layout = shader->GetUniformBufferLayout(block);

			material_uniform_buffer = OpenGLUniformBuffer::Create("Material", layout);
			material_uniform_buffer->SetVec4("color", { 1.0f, 1.0f, 1.0f, 1.0f });
			shader->AddUniformBuffer(material_uniform_buffer);
		}
		material_uniform_buffer->SetVec4("color", color);
	}

	void MeshMaterial::Draw() const {
		material_uniform_buffer->SetVec4("color", color);
		shader->Bind();
	}

	WorldMaterial::WorldMaterial() {
		shader = ShaderLibrary::Get().Load("world");
	}

	void WorldMaterial::Draw() const {
		shader->Bind();
	}
}