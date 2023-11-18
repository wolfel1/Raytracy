#include "raytracypch.h"
#include "Mesh.h"

#include "../Renderer.h"

namespace raytracy {

	Mesh::Mesh(glm::vec3 const& position) : origin(position) {
	}

	void Mesh::Init(shared_ptr<MeshData> const mesh_data)  {

		vertex_array = VertexArray::Create();

		vertex_buffer = VertexBuffer::Create(mesh_data->vertices);
		vertex_buffer->SetLayout({
			{ "position", VertexDataType::Float3 },
			{ "color", VertexDataType::Float4 }
		});

		index_buffer = IndexBuffer::Create(mesh_data->indices.data(), static_cast<uint32_t>(mesh_data->indices.size()));

		shader = Shader::CreateFromDirectory("basic");
		RTY_ASSERT(shader, "Could not create a shader program!"); 
		auto shading_uniform_buffer = UniformBuffer::Create("Shading", {
			{ "color", VertexDataType::Float4 },
			{ "light", VertexDataType::Float3 }
		});
		shading_uniform_buffer->SetVec4("color", display_color);
		shading_uniform_buffer->SetVec3("light", light_color);
		shader->AddUniformBuffer("shading", shading_uniform_buffer);

		shader->AddUniformBuffer("camera", Renderer::Get().GetCameraUniformBuffer());

		vertex_array->SetVertexBuffer(vertex_buffer);
		vertex_array->SetIndexBuffer(index_buffer);

		RTY_RENDERER_TRACE("Mesh created with type {0}.", mesh_data->name);
	}

	void Mesh::SetDisplayColor(glm::vec4 const& color) {
		display_color = color;
		shader->Bind();
		auto& uniform_buffers = shader->GetUniformBuffers();
		auto it = uniform_buffers.find("shading");
		RTY_ASSERT(it != uniform_buffers.end(), "No uniform buffer with key 'shading' exists!");
		it->second->SetVec4("color", color);
		shader->Unbind();
	}

	void Mesh::Translate(glm::vec3 const& direction) {
		origin += direction;
		model_matrix = glm::translate(glm::mat4(1.0f), direction) * model_matrix;
	}

	void Mesh::Scale(float const value) {
		model_matrix = model_matrix * glm::scale(glm::mat4(1.0f), glm::vec3(value));
	}

	Plane::Plane() {
		auto data = make_shared<PlaneData>();
		data->Init();
		Init(data);
	}

	Plane::Plane(glm::vec3 const& position, float const scale_factor) : Mesh(position) {
		auto data = make_shared<PlaneData>();
		data->Init(position, scale_factor);
		Init(data);
	}

    Cube::Cube() {
		auto data = make_shared<CubeData>();
		data->Init();
		Init(data);
    }

	Cube::Cube(glm::vec3 const& position, float const scale_factor) {
		auto data = make_shared<CubeData>();
		data->Init(position, scale_factor);
		Init(data);
	}
}