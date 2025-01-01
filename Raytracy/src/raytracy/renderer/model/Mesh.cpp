#include "raytracypch.h"
#include "Mesh.h"

#include "../Renderer.h"
#include "../ViewportScene.h"
#include "../api/Shader.h"

#include <glad/gl.h>

namespace raytracy::renderer {

	shared_ptr<Material> Mesh::default_material = nullptr;

	Mesh::Mesh(shared_ptr<MeshData> const mesh_data, glm::vec3 const& position, float const scale_factor) {
		Init(mesh_data);
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

		AddDefaultMaterial();

		if (mesh_data->is_indexed) {
			auto index_buffer = OpenGLIndexBuffer::Create(mesh_data->indices.data(), static_cast<uint32_t>(mesh_data->indices.size()));
			vertex_array->SetIndexBuffer(index_buffer);
			is_indexed = true;
		}

#if RAYTRACING
		BuildTriangles(mesh_data);
		BuildBoundingBox(mesh_data);
#endif

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

	void Mesh::AddDefaultMaterial() {
		if (!default_material) {
			default_material = make_shared<MeshMaterial>();
		}

		material = default_material;
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
			bounding_box.min_corner = glm::min(bounding_box.min_corner, glm::vec3(model_matrix * glm::vec4(vertex.position, 1.0f)));
			bounding_box.max_corner = glm::max(bounding_box.max_corner, glm::vec3(model_matrix * glm::vec4(vertex.position, 1.0f)));
		}
	}

	void Mesh::UpdateBoundingBox(glm::mat4 const& transformation_matrix) {
#if RAYTRACING
		bounding_box.min_corner = glm::vec3(transformation_matrix * glm::vec4(bounding_box.min_corner, 1.0f));
		bounding_box.max_corner = glm::vec3(transformation_matrix * glm::vec4(bounding_box.max_corner, 1.0f));

		for (auto& node : bounding_volume_hierarchie) {
			node.min_corner = glm::vec3(transformation_matrix * glm::vec4(node.min_corner, 1.0f));
			node.max_corner = glm::vec3(transformation_matrix * glm::vec4(node.max_corner, 1.0f));
		}

		// TODO: Update the global bounding volume hierarchie
#endif
	}

	void Mesh::BuildBoundingVolumeHierarchie() {
		bounding_volume_hierarchie.clear();
		triangles.clear();

		BoundingBoxNode& root = bounding_volume_hierarchie.emplace_back();

		root.object_indices.resize(triangles.size());
		for (size_t i = 0; i < triangles.size(); i++) {
			root.object_indices[i] = static_cast<uint32_t>(i);
		}

		UpdateBounds(0);
		Subdivide(0);
	}

	void Mesh::UpdateBounds(uint32_t node_index) {
		BoundingBoxNode& node = bounding_volume_hierarchie[node_index];
		node.min_corner = glm::vec3(infinity);
		node.max_corner = glm::vec3(-infinity);

		for (size_t i = 0; i < node.object_indices.size(); i++) {
			auto triangle = triangles[node.object_indices[i]];
			auto bounding_box = GetTriangleBoundingBox(*triangle);
			node.min_corner = glm::min(node.min_corner, bounding_box.min_corner);
			node.max_corner = glm::max(node.max_corner, bounding_box.max_corner);
		}
	}

	BoundingBox Mesh::GetTriangleBoundingBox(Triangle& triangle) {
		BoundingBox bounding_box;

		for (auto& vertex : triangle.vertices) {
			bounding_box.min_corner = glm::min(bounding_box.min_corner, glm::vec3(model_matrix * glm::vec4(vertex->position, 1.0f))) - glm::epsilon<float>();
			bounding_box.max_corner = glm::max(bounding_box.max_corner, glm::vec3(model_matrix * glm::vec4(vertex->position, 1.0f)))+ glm::epsilon<float>();
		}

		return bounding_box;
	}

	void Mesh::Subdivide(uint32_t node_index) {
		BoundingBoxNode node = bounding_volume_hierarchie[node_index];
		if (node.object_indices.size() <= 2) {
			return;
		}

		glm::vec3 volume_extent = node.max_corner - node.min_corner;
		uint16_t split_axis = 0;
		if (volume_extent[1] > volume_extent[0]) {
			split_axis = 1;
		}
		if (volume_extent[2] > volume_extent[split_axis]) {
			split_axis = 2;
		}
		auto split_position = node.min_corner[split_axis] + volume_extent[split_axis] / 2;

		bounding_volume_hierarchie.emplace_back();
		auto left_child_index = static_cast<uint32_t>(bounding_volume_hierarchie.size() - 1);
		BoundingBoxNode& right_child = bounding_volume_hierarchie.emplace_back();
		auto right_child_index = static_cast<uint32_t>(bounding_volume_hierarchie.size() - 1);

		BoundingBoxNode& left_child = bounding_volume_hierarchie[left_child_index];
		for (auto object_index : node.object_indices) {
			if (triangles[object_index]->GetCenter()[split_axis] < split_position) {
				left_child.object_indices.push_back(object_index);
			} else {
				right_child.object_indices.push_back(object_index);
			}
		}

		node.object_indices.clear();
		node.left_child_index = static_cast<uint32_t>(left_child_index);
		node.right_child_index = static_cast<uint32_t>(right_child_index);

		bounding_volume_hierarchie[node_index] = node;

		UpdateBounds(left_child_index);
		UpdateBounds(right_child_index);
		Subdivide(left_child_index);
		Subdivide(right_child_index);
	}


	QuadData Plane::data;
	Plane::Plane(glm::vec3 const position, float const scale_factor) : Mesh(make_shared<MeshData>(data), position, scale_factor) {
		Rotate({ 1.0f, 0.0f, 0.0f }, glm::radians(-90.0f));
	}

	CubeData Cube::data;
	Cube::Cube(glm::vec3 const position, float const scale_factor) : Mesh(make_shared<MeshData>(data), position, scale_factor) {
	}

	SphereData Sphere::data;
	Sphere::Sphere(glm::vec3 const position, float const scale_factor) : Mesh(make_shared<MeshData>(data), position, scale_factor) {
	}

	CubeData Skybox::data;
	Skybox::Skybox() {
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