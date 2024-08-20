#include "raytracypch.h"
#include "Mesh.h"

#include "../Renderer.h"
#include "../ViewportScene.h"
#include "../api/Shader.h"

#include <glad/gl.h>

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

		material = make_shared<MeshMaterial>();

		if (mesh_data->is_indexed) {
			auto index_buffer = OpenGLIndexBuffer::Create(mesh_data->indices.data(), static_cast<uint32_t>(mesh_data->indices.size()));
			vertex_array->SetIndexBuffer(index_buffer);
			is_indexed = true;
		}

		BuildTriangles(mesh_data);
		BuildBoundingBox(mesh_data);

		RTY_RENDERER_TRACE("Mesh created with type {0}.", mesh_data->name);
	}



	void Mesh::Draw(shared_ptr<OpenGLRendererAPI> api) {
		material->Draw();

		vertex_array->Bind();

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

	void Mesh::BuildTriangles(shared_ptr<MeshData> const mesh_data) {
		if (mesh_data->is_indexed) {
			RTY_ASSERT(mesh_data->indices.size() % 3 == 0, "Can not build triangles!");

			auto& indices = mesh_data->indices;
			auto& vertices = mesh_data->vertices;
			for (auto i = 0; i < indices.size(); i += 3) {
				auto triangle = make_shared<Triangle>(
					make_shared<Vertex>(vertices[indices[i]]),
					make_shared<Vertex>(vertices[indices[i + 1]]),
					make_shared<Vertex>(vertices[indices[i + 2]])
				);

				triangles.push_back(triangle);
			}

		} else {
			RTY_ASSERT(mesh_data->vertices.size() % 3 == 0, "Can not build triangles!");

			for (auto i = 0; i < mesh_data->vertices.size(); i += 3) {
				auto triangle = make_shared<Triangle>(
					make_shared<Vertex>(mesh_data->vertices[i]),
					make_shared<Vertex>(mesh_data->vertices[i + 1]),
					make_shared<Vertex>(mesh_data->vertices[i + 2])
				);

				triangles.push_back(triangle);
			}
		}
	}

	void Mesh::BuildBoundingBox(shared_ptr<MeshData> const mesh_data) {
		for (auto& vertex : mesh_data->vertices) {
			bounding_box.min_corner = glm::min(bounding_box.min_corner, vertex.position);
			bounding_box.max_corner = glm::max(bounding_box.max_corner, vertex.position);
		}
	}

	QuadData Plane::data;
	Plane::Plane(glm::vec3 const position, float const scale_factor) : Mesh(position, scale_factor) {
		Init(make_shared<MeshData>(data));
		Rotate({ 1.0f, 0.0f, 0.0f }, glm::radians(-90.0f));
	}

	CubeData Cube::data;
	Cube::Cube(glm::vec3 const position, float const scale_factor) : Mesh(position, scale_factor) {
		Init(make_shared<MeshData>(data));
	}

	SphereData Sphere::data;
	Sphere::Sphere(glm::vec3 const position, float const scale_factor) : Mesh(position, scale_factor) {
		Init(make_shared<MeshData>(data));
	}

	CubeData Skybox::data;
	Skybox::Skybox(glm::vec3 const position, float const scale_factor) {
		Init(make_shared<MeshData>(data));
	}

	void Skybox::Draw(shared_ptr<OpenGLRendererAPI> api) {
		material->Draw();
		vertex_array->Bind();

		api->SetFrontFace(FrontFace::INSIDE);

		api->DrawIndexed(vertex_array);

		api->SetFrontFace(FrontFace::OUTSIDE);
	}
}