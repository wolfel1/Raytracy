#include "raytracypch.h"
#include "Material.h"

#include "../api/Shader.h"
#include "../ViewportScene.h"

namespace raytracy::renderer {

	MeshMaterial::MeshMaterial() {
		shader = ShaderLibrary::Get().Load("default");
		RTY_ASSERT(shader, "Could not create a shader program!");

		UniformBlock block("Material", {
			"color",
			"specular",
			"shininess"
		});

		auto layout = shader->GetUniformBufferLayout(block);

		material_uniform_buffer = OpenGLUniformBuffer::Create("Material", layout);
		shader->AddUniformBuffer(material_uniform_buffer);
		
		material_uniform_buffer->SetVec4("color", color);
		material_uniform_buffer->SetVec3("specular", specular);
		material_uniform_buffer->SetFloat("shininess", shininess);
	}

	void MeshMaterial::Draw() const {
		shader->Bind();
		shader->BindBuffer(material_uniform_buffer);
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