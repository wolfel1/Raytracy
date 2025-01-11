#include "raytracypch.h"
#include "Mesh.h"

#include "../Renderer.h"
#include "../ViewportScene.h"
#include "../api/Shader.h"

#include <glad/gl.h>

namespace raytracy::renderer {

	void Mesh::Init(MeshData& data) {
		RTY_PROFILE_FUNCTION();

		CreateVertexContainer(data);

		mesh_data = std::move(data);
#if RAYTRACING
		BuildTriangles();
		BuildBoundingBox();
#endif
		material = MaterialLibrary::Get().Load("DefaultMaterial");

		RTY_RENDERER_TRACE("Mesh created with type {0}.", mesh_data.name);
	}

	void Mesh::CreateVertexContainer(MeshData const& mesh_data) {
		vertex_array = OpenGLVertexArray::Create();

		auto vertex_buffer = OpenGLVertexBuffer::Create(mesh_data.vertices);
		vertex_buffer->SetLayout({
			{ "position", VertexDataType::Float3 },
			{ "normal", VertexDataType::Float3 },
			{ "color", VertexDataType::Float4 },
			{ "tex_coords", VertexDataType::Float2 }
		});

		vertex_array->SetVertexBuffer(vertex_buffer);

		if (mesh_data.is_indexed) {
			auto index_buffer = OpenGLIndexBuffer::Create(mesh_data.indices.data(), static_cast<uint32_t>(mesh_data.indices.size()));
			vertex_array->SetIndexBuffer(index_buffer);
		}
	}

	void Mesh::Draw(shared_ptr<OpenGLRendererAPI> api) {
		material->Draw();

		vertex_array->Bind();

		if (mesh_data.is_indexed) {
			api->DrawIndexed(vertex_array);
		} else {
			api->Draw(vertex_array);
		}
	}

	void Mesh::Translate(glm::vec3 const& direction) {
		auto translation_matrix = glm::translate(glm::mat4(1.0f), direction);
		UpdateBoundingBox(translation_matrix);

		origin += direction;
		model_matrix = translation_matrix * model_matrix;
	}

	void Mesh::Rotate(glm::vec3 const& axis, float const value) {
		auto rotation_matrix = glm::rotate(glm::mat4(1.0f), value, axis);
		UpdateBoundingBox(rotation_matrix);

		model_matrix = model_matrix * rotation_matrix;
	}

	void Mesh::Scale(float const value) {
		auto scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(value));
		UpdateBoundingBox(scale_matrix);

		scale *= value;
		model_matrix = model_matrix * scale;
	}


	void Mesh::BuildTriangles() {
		RTY_PROFILE_FUNCTION();
		if (mesh_data.is_indexed) {
			RTY_ASSERT(mesh_data.indices.size() % 3 == 0, "Can not build triangles!");

			auto& indices = mesh_data.indices;
			auto& vertices = mesh_data.vertices;
			triangles.reserve(indices.size() / 3);

			for (auto i = 0; i < indices.size(); i += 3) {
				auto& triangle = triangles.emplace_back();
					
				triangle.vertices[0] = vertices[indices[i]];
				triangle.vertices[1] = vertices[indices[i + 1]];
				triangle.vertices[2] = vertices[indices[i + 2]];
				
				triangle.center = (vertices[indices[i]].position + vertices[indices[i + 1]].position + vertices[indices[i + 2]].position) / 3.0f;
			}

		} else {
			RTY_ASSERT(mesh_data.vertices.size() % 3 == 0, "Can not build triangles!");

			for (uint32_t i = 0; i < mesh_data.vertices.size(); i += 3) {
				auto& triangle = triangles.emplace_back();
				triangle.vertices[0] = mesh_data.vertices[i];
				triangle.vertices[1] = mesh_data.vertices[i + 1];
				triangle.vertices[2] = mesh_data.vertices[i + 2];
				
				triangle.center = (mesh_data.vertices[i].position + mesh_data.vertices[i + 1].position + mesh_data.vertices[i + 2].position) / 3.0f;
			}
		}
	}

	void Mesh::BuildBoundingBox() {
		RTY_PROFILE_FUNCTION();
		for (auto const& vertex : mesh_data.vertices) {
			bounding_box.min_corner = glm::min(bounding_box.min_corner, vertex.position);
			bounding_box.max_corner = glm::max(bounding_box.max_corner, vertex.position);
		}
	}

	void Mesh::UpdateBoundingBox(glm::mat4 const& transformation_matrix) {
		RTY_PROFILE_FUNCTION();
#if RAYTRACING
		bounding_box.min_corner = glm::vec3(transformation_matrix * glm::vec4(bounding_box.min_corner, 1.0f));
		bounding_box.max_corner = glm::vec3(transformation_matrix * glm::vec4(bounding_box.max_corner, 1.0f));

		for (auto& triangle : triangles) {
			std::for_each(std::begin(triangle.vertices), std::end(triangle.vertices), [&](auto& vertex) {
				vertex.position = glm::vec3(transformation_matrix * glm::vec4(vertex.position, 1.0f));
			});
		}
		// TODO: Update the global bounding volume hierarchie
#endif
	}

	Plane::Plane(glm::vec3 const& position, float const scale_factor) : Mesh(position, scale_factor) {
		RTY_PROFILE_FUNCTION();
		MeshData data = MeshProvider::GetPlaneData(scale_factor, position);
		Init(data);
	}

	Cube::Cube(glm::vec3 const position, float const scale_factor) : Mesh(position, scale_factor) {
		RTY_PROFILE_FUNCTION();
		MeshData data = MeshProvider::GetCubeData(scale_factor, position);
		Init(data);
	}

	Sphere::Sphere(glm::vec3 const position, float const scale_factor) : Mesh(position, scale_factor) {
		RTY_PROFILE_FUNCTION();
		MeshData data = MeshProvider::GetSphereData(scale_factor, position);
		Init(data);
	}

	Skybox::Skybox() {
		MeshData data = MeshProvider::GetCubeData();
		CreateVertexContainer(data);
	}

	void Skybox::Draw(shared_ptr<OpenGLRendererAPI> api) {
		material->Draw();
		vertex_array->Bind();

		api->SetFrontFace(FrontFace::INSIDE);

		api->DrawIndexed(vertex_array);

		api->SetFrontFace(FrontFace::OUTSIDE);
	}
}