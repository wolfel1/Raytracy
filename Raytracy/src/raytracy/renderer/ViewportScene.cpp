#include "raytracypch.h"
#include "ViewportScene.h"

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
		bounding_volume_hierarchie.clear();

		BoundingBoxNode& root = bounding_volume_hierarchie.emplace_back();

		root.triangle_indices.resize(triangles.size());
		for (size_t i = 0; i < triangles.size(); i++) {
			root.triangle_indices[i] = static_cast<uint32_t>(i);
		}

		UpdateBounds(0);
		Subdivide(0);
	}

	void Scene::UpdateBounds(uint32_t node_index) {
		BoundingBoxNode& node = bounding_volume_hierarchie[node_index];
		node.min_corner = glm::vec3(infinity);
		node.max_corner = glm::vec3(-infinity);

		for (size_t i = 0; i < node.triangle_indices.size(); i++) {
			auto triangle = triangles[node.triangle_indices[i]];
			auto bounding_box = GetTriangleBoundingBox(*triangle);
			node.min_corner = glm::min(node.min_corner, bounding_box.min_corner);
			node.max_corner = glm::max(node.max_corner, bounding_box.max_corner);
		}
	}

	BoundingBox Scene::GetTriangleBoundingBox(Triangle& triangle) {
		BoundingBox bounding_box;

		for (auto& vertex : triangle.vertices) {
			bounding_box.min_corner = glm::min(bounding_box.min_corner, vertex->position);
			bounding_box.max_corner = glm::max(bounding_box.max_corner, vertex->position);
		}

		return bounding_box;
	}

	void Scene::Subdivide(uint32_t node_index) {
		BoundingBoxNode node = bounding_volume_hierarchie[node_index];
		if (node.triangle_indices.size() <= 2) {
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

		BoundingBoxNode left_child, right_child;

		for (auto triangle_index : node.triangle_indices) {
			auto triangle = triangles[triangle_index];
			if (triangle->GetCenter()[split_axis] < split_position) {
				left_child.triangle_indices.push_back(triangle_index);
			} else {
				right_child.triangle_indices.push_back(triangle_index);
			}
		}

		if (right_child.triangle_indices.empty() || left_child.triangle_indices.empty()) {
			return;
		}

		node.triangle_indices.clear();
		node.left_child_index = static_cast<uint32_t>(bounding_volume_hierarchie.size());
		bounding_volume_hierarchie.emplace_back(left_child);
		node.right_child_index = static_cast<uint32_t>(bounding_volume_hierarchie.size());
		bounding_volume_hierarchie.emplace_back(right_child);

		bounding_volume_hierarchie[node_index] = node;

		UpdateBounds(node.left_child_index);
		UpdateBounds(node.right_child_index);
		//Subdivide(node.left_child_index);
		//Subdivide(node.right_child_index);
	}
}
