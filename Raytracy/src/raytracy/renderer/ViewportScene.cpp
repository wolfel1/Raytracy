#include "raytracypch.h"
#include "ViewportScene.h"

namespace raytracy::renderer {

	shared_ptr<Scene> Scene::instance = nullptr;

	void Scene::Create(std::shared_ptr<PerspectiveCamera> camera) {
		if (!instance) {
			instance = shared_ptr<Scene>(new Scene(camera));
		} else {
			RTY_RENDERER_ERROR("Multiple Scenes are not supported yet!");
		}
	}

	Scene::Scene(std::shared_ptr<PerspectiveCamera> camera) : camera(camera) {

	}

	void Scene::AddMesh(std::shared_ptr<Mesh> const mesh) {
		meshes.push_back(mesh);

		BuildBoundingVolumeHierarchie();
	}

	void Scene::AddSkybox() {
		skybox = make_shared<Skybox>();
		auto skybox_material = make_shared<WorldMaterial>();
		skybox->SetMaterial(skybox_material);
	}


	void Scene::BuildBoundingVolumeHierarchie() {
		bounding_volume_hierarchie.clear();
		triangles.clear();

		BoundingBoxNode& root = bounding_volume_hierarchie.emplace_back();
		for (auto mesh : meshes) {
			auto& mesh_triangles = mesh->GetTriangles();
			triangles.insert(std::end(triangles), std::begin(mesh_triangles), std::end(mesh_triangles));
		}

		root.object_indices.resize(triangles.size());
		for (size_t i = 0; i < triangles.size(); i++) {
			root.object_indices[i] = i;
		}

		UpdateBounds(0);
		Subdivide(0);
	}

	void Scene::UpdateBounds(uint32_t node_index) {
		BoundingBoxNode& node = bounding_volume_hierarchie[node_index];
		node.min_corner = glm::vec3(infinity);
		node.max_corner = glm::vec3(-infinity);

		for (size_t i = 0; i < node.object_indices.size(); i++) {
			auto triangle = triangles[node.object_indices[i]];
			for (auto vertex : triangle->vertices) {
				node.min_corner = glm::min(node.min_corner, vertex->position);
				node.max_corner = glm::max(node.max_corner, vertex->position);
			}

		}
	}

	void Scene::Subdivide(uint32_t node_index) {
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
		auto left_child_index = bounding_volume_hierarchie.size() - 1;
		BoundingBoxNode& right_child = bounding_volume_hierarchie.emplace_back();
		auto right_child_index = bounding_volume_hierarchie.size() - 1;

		BoundingBoxNode& left_child = bounding_volume_hierarchie[left_child_index];
		for (size_t object_index : node.object_indices) {
			if (triangles[object_index]->center[split_axis] < split_position) {
				left_child.object_indices.push_back(object_index);
			} else {
				right_child.object_indices.push_back(object_index);
			}
		}

		node.object_indices.clear();
		node.left_child_index = left_child_index;
		node.right_child_index = right_child_index;

		bounding_volume_hierarchie[node_index] = node;

		UpdateBounds(left_child_index);
		UpdateBounds(right_child_index);
		Subdivide(left_child_index);
		Subdivide(right_child_index);
	}
}
