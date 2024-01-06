#include "raytracypch.h"
#include "Mesh.h"

#include "../Renderer.h"
#include "../ViewportScene.h"
#include "../api/Shader.h"

namespace raytracy::renderer {

	Mesh::Mesh(glm::vec3 const& position, float const scale_factor) {
		Translate(position);
		Scale(scale_factor);
	}

	Mesh::~Mesh() {
		material->Destroy();
	}

	void Mesh::Init(shared_ptr<MeshData> const mesh_data) {
		RTY_ASSERT(Scene::Get(), "Must have scene to create meshes!")

		vertex_array = OpenGLVertexArray::Create();

		auto vertex_buffer = OpenGLVertexBuffer::Create(mesh_data->vertices);
		vertex_buffer->SetLayout({
			{ "position", VertexDataType::Float3 },
			{ "normal", VertexDataType::Float3 },
			{ "color", VertexDataType::Float4 },
			{ "tex_coords", VertexDataType::Float2 }
		});

		vertex_array->SetVertexBuffer(vertex_buffer);

		material = make_shared<Material>();

		if (mesh_data->is_indexed) {
			auto index_buffer = OpenGLIndexBuffer::Create(mesh_data->indices.data(), static_cast<uint32_t>(mesh_data->indices.size()));
			vertex_array->SetIndexBuffer(index_buffer);
			is_indexed = true;
		}

		RTY_RENDERER_TRACE("Mesh created with type {0}.", mesh_data->name);
	}

	void Mesh::Draw(shared_ptr<OpenGLRendererAPI> api) {
		material->Draw();

		vertex_array->Bind();
		material->GetShader()->Bind();

		if (is_indexed) {
			api->DrawIndexed(vertex_array);
		} else {
			api->Draw(vertex_array);
		}
	}

	void Mesh::Translate(glm::vec3 const& direction) {
		origin += direction;
		model_matrix = glm::translate(glm::mat4(1.0f), direction) * model_matrix;
	}

	void Mesh::Rotate(glm::vec3 const& axis, float const value) {
		model_matrix = model_matrix * glm::rotate(glm::mat4(1.0f), value, axis);
	}

	void Mesh::Scale(float const value) {
		scale *= value;
		model_matrix = model_matrix * glm::scale(glm::mat4(1.0f), glm::vec3(value));
	}

	QuadData Plane::data;

	Plane::Plane(glm::vec3 const position, float const scale_factor) : Mesh(position, scale_factor) {
		Init(make_shared<MeshData>(data));
		Rotate({ 1.0f, 0.0f, 0.0f }, glm::radians(-90.0f));
	}

	FlatCubeData Cube::data;
	Cube::Cube(glm::vec3 const position, float const scale_factor) : Mesh(position, scale_factor) {
		Init(make_shared<MeshData>(data));
	}

	SphereData Sphere::data;
	Sphere::Sphere(glm::vec3 const position, float const scale_factor) : Mesh(position, scale_factor) {
		Init(make_shared<MeshData>(data));
	}


	
}