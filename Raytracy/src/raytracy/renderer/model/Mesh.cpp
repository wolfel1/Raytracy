#include "raytracypch.h"
#include "Mesh.h"

#include "../Renderer.h"

namespace raytracy::renderer {

	Mesh::Mesh(glm::vec3 const& position, float const scale_factor) {
		Translate(position);
		Scale(scale_factor);
	}

	void Mesh::Init(shared_ptr<MeshData> const mesh_data) {

		vertex_array = VertexArray::Create();

		auto vertex_buffer = VertexBuffer::Create(mesh_data->vertices);
		vertex_buffer->SetLayout({
			{ "position", VertexDataType::Float3 },
			{ "normal", VertexDataType::Float3 },
			{ "color", VertexDataType::Float4 }
		});

		vertex_array->SetVertexBuffer(vertex_buffer);

		if (mesh_data->is_indexed) {
			auto index_buffer = IndexBuffer::Create(mesh_data->indices.data(), static_cast<uint32_t>(mesh_data->indices.size()));
			vertex_array->SetIndexBuffer(index_buffer);
		}

		shader = ShaderLibrary::Get().Load("basic");
		RTY_ASSERT(shader, "Could not create a shader program!");

		auto layout = shader->GetUniformBufferLayout("Shading");
		shading_uniform_buffer = UniformBuffer::Create("Shading", layout);
		shading_uniform_buffer->SetVec4("display_color", display_color);
		shading_uniform_buffer->SetVec3("light_color", light_color);
		shading_uniform_buffer->SetVec3("light_position", light_position);

		shader->AddUniformBuffer("Shading", shading_uniform_buffer);

		RTY_RENDERER_TRACE("Mesh created with type {0}.", mesh_data->name);
		this->mesh_data = mesh_data;
	}

	void Mesh::SetDisplayColor(glm::vec4 const& color) {
		display_color = color;
		shading_uniform_buffer->SetVec4("display_color", color);
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

	Plane::Plane(glm::vec3 const& position, float const scale_factor) : Mesh(position, scale_factor) {
		auto data = make_shared<PlaneData>();
		data->Init();
		Init(data);
	}

    Cube::Cube() {
		auto data = make_shared<CubeData>();
		data->Init();
		Init(data);
    }

	Cube::Cube(glm::vec3 const& position, float const scale_factor) : Mesh(position, scale_factor) {
		auto data = make_shared<CubeData>();
		data->Init();
		Init(data);
	}
}