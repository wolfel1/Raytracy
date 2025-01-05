#include "raytracypch.h"
#include "ViewportScene.h"

#include <numeric>

namespace raytracy::renderer {

	shared_ptr<Scene> Scene::instance = nullptr;

	void Scene::Create(std::shared_ptr<PerspectiveCamera> camera) {
		if (!instance) {
			instance = make_shared<Scene>(camera);
		} else {
			RTY_RENDERER_ERROR("Multiple Scenes are not supported yet!");
		}
	}

	Scene::Scene(std::shared_ptr<PerspectiveCamera> camera) : camera(camera) {
		light = make_shared<DirectionalLight>();
	}

	void Scene::AddMesh(std::shared_ptr<Mesh> const mesh) {
		RTY_PROFILE_FUNCTION();
		meshes.push_back(mesh);

#if RAYTRACING
		AddTriangles(mesh);
		BuildBoundingVolumeHierarchie();
#endif
	}

	void Scene::AddTriangles(std::shared_ptr<Mesh> const mesh) {
		auto& mesh_triangles = mesh->GetTriangles();
		triangles.insert(std::end(triangles), std::begin(mesh_triangles), std::end(mesh_triangles));
	}

	void Scene::AddSkybox() {
		skybox = make_shared<Skybox>();
		auto skybox_material = make_shared<WorldMaterial>();
		skybox->SetMaterial(skybox_material);
	}


	void Scene::BuildBoundingVolumeHierarchie() {
		RTY_PROFILE_FUNCTION();
		bounding_volume_hierarchie.clear();

		BoundingBoxNode& root = bounding_volume_hierarchie.emplace_back();

		root.triangle_indices.resize(triangles.size());
		std::iota(root.triangle_indices.begin(), root.triangle_indices.end(), 0);

		UpdateBounds(0);
		Subdivide(0);
	}
	
	inline static float snapToGrid(float value) {
		constexpr float gridSize = glm::epsilon<float>();
		return std::round(value / gridSize) * gridSize;
	}

	inline glm::vec3 snapToGrid(const glm::vec3& value) {
		return glm::vec3(
			snapToGrid(value.x),
			snapToGrid(value.y),
			snapToGrid(value.z)
		);
	}

	void Scene::UpdateBounds(uint32_t node_index) {
		BoundingBoxNode& node = bounding_volume_hierarchie[node_index];
		node.min_corner = glm::vec3(infinity);
		node.max_corner = glm::vec3(-infinity);

		for (size_t i = 0; i < node.triangle_indices.size(); i++) {
			auto triangle = triangles[node.triangle_indices[i]];

			std::for_each(triangle->vertices.begin(), triangle->vertices.end(), [&](auto& vertex) {
				node.min_corner = glm::min(node.min_corner, snapToGrid(vertex->position));
				node.max_corner = glm::max(node.max_corner, snapToGrid(vertex->position));
			});
		}
	}

	void Scene::Subdivide(uint32_t node_index) {
		RTY_PROFILE_FUNCTION();
		BoundingBoxNode node = bounding_volume_hierarchie[node_index];
		if (node.triangle_indices.size() < 2) {
			return;
		}

		glm::vec3 volume_extent = node.max_corner - node.min_corner;
		uint16_t split_axis = 0;
		if (volume_extent[1] > volume_extent[split_axis]) {
			split_axis = 1;
		}
		if (volume_extent[2] > volume_extent[split_axis]) {
			split_axis = 2;
		}
		auto split_position = node.min_corner[split_axis] + volume_extent[split_axis] / 2;

		std::vector<uint32_t> left_child_triangle_indices, right_child_triangle_indices;

		std::for_each(node.triangle_indices.begin(), node.triangle_indices.end(), [&](auto& index) {
			auto triangle = triangles[index];
			if (triangle->GetCenter()[split_axis] < split_position) {
				left_child_triangle_indices.emplace_back(index);
			} else {
				right_child_triangle_indices.emplace_back(index);
			}
		});

		if (right_child_triangle_indices.empty() || left_child_triangle_indices.empty()) {
			return;
		}

		bounding_volume_hierarchie.reserve(bounding_volume_hierarchie.size() + 2);
		auto left_child_index = static_cast<uint32_t>(bounding_volume_hierarchie.size());
		auto& left_child = bounding_volume_hierarchie.emplace_back();
		auto right_child_index = static_cast<uint32_t>(bounding_volume_hierarchie.size());
		auto& right_child = bounding_volume_hierarchie.emplace_back();

		left_child.triangle_indices = left_child_triangle_indices;
		right_child.triangle_indices = right_child_triangle_indices;

		node.left_child_index = left_child_index;
		node.right_child_index = right_child_index;
		node.triangle_indices.clear();
		bounding_volume_hierarchie[node_index] = node;

		UpdateBounds(left_child_index);
		UpdateBounds(right_child_index);
		Subdivide(node.left_child_index);
		Subdivide(node.right_child_index);
	}
}
