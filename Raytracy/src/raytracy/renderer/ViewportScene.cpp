#include "raytracypch.h"
#include "ViewportScene.h"

#include <numeric>

namespace raytracy::renderer {

	shared_ptr<Scene> Scene::instance = nullptr;

	std::shared_ptr<Scene> Scene::Create(std::shared_ptr<PerspectiveCamera> camera) {
		if (!instance) {
			instance = make_shared<Scene>(camera);
		} else {
			RTY_RENDERER_ERROR("Multiple Scenes are not supported yet!");
		}
		return instance;
	}

	Scene::Scene(std::shared_ptr<PerspectiveCamera> camera) : camera(camera) {
		light = make_shared<DirectionalLight>();
	}

	void Scene::AddMesh(std::shared_ptr<Mesh> const mesh) {
		RTY_PROFILE_FUNCTION();
		meshes.push_back(mesh);

#if RAYTRACING
		BuildBoundingVolumeHierarchie();
#endif
	}


	void Scene::AddSkybox() {
		skybox = make_shared<Skybox>();
		auto skybox_material = make_shared<WorldMaterial>();
		skybox->SetMaterial(skybox_material);
	}


	void Scene::BuildBoundingVolumeHierarchie() {
		RTY_PROFILE_FUNCTION();
		bounding_volume_hierarchie.clear();
		triangles.clear();

		BoundingBoxNode& root = bounding_volume_hierarchie.emplace_back();

		root.mesh_indices.resize(meshes.size());
		std::iota(std::begin(root.mesh_indices), std::end(root.mesh_indices), 0);

		UpdateMeshBounds(0);
		SubdivideMeshes(0);
	}
	
	

	void Scene::UpdateMeshBounds(uint32_t node_index) {
		BoundingBoxNode& node = bounding_volume_hierarchie[node_index];
		node.min_corner = glm::vec3(infinity);
		node.max_corner = glm::vec3(-infinity);

		for (const auto index : node.mesh_indices) {
			auto mesh = meshes[index];
			auto& bounding_box = mesh->GetBoundingBox();
			node.min_corner = glm::min(node.min_corner, snapToGrid(bounding_box.min_corner));
			node.max_corner = glm::max(node.max_corner, snapToGrid(bounding_box.max_corner));
		}
	}

	void Scene::SubdivideMeshes(uint32_t node_index) {
		RTY_PROFILE_FUNCTION();
		BoundingBoxNode node = bounding_volume_hierarchie[node_index];
		if (node.mesh_indices.size() < 2) {
			BuildTriangleBoundingVolumeHierarchie(node_index);
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

		std::vector<uint32_t> left_child_mesh_indices, right_child_mesh_indices;

		for (const auto index : node.mesh_indices) {
			auto mesh = meshes[index];
			if (mesh->GetOrigin()[split_axis] < split_position) {
				left_child_mesh_indices.emplace_back(index);
			} else {
				right_child_mesh_indices.emplace_back(index);
			}
		}

		if (right_child_mesh_indices.empty() || left_child_mesh_indices.empty()) {
			BuildTriangleBoundingVolumeHierarchie(node_index);
			return;
		}

		bounding_volume_hierarchie.reserve(bounding_volume_hierarchie.size() + 2);
		auto left_child_index = static_cast<uint32_t>(bounding_volume_hierarchie.size());
		auto& left_child = bounding_volume_hierarchie.emplace_back();
		auto right_child_index = static_cast<uint32_t>(bounding_volume_hierarchie.size());
		auto& right_child = bounding_volume_hierarchie.emplace_back();

		left_child.mesh_indices = std::move(left_child_mesh_indices);
		right_child.mesh_indices = std::move(right_child_mesh_indices);

		node.left_child_index = left_child_index;
		node.right_child_index = right_child_index;
		node.mesh_indices.clear();
		bounding_volume_hierarchie[node_index] = std::move(node);

		UpdateMeshBounds(left_child_index);
		UpdateMeshBounds(right_child_index);
		SubdivideMeshes(left_child_index);
		SubdivideMeshes(right_child_index);
	}

	void Scene::BuildTriangleBoundingVolumeHierarchie(uint32_t node_index) {
		RTY_PROFILE_FUNCTION();

		BoundingBoxNode& root = bounding_volume_hierarchie[node_index];

		for (const auto index : root.mesh_indices) {
			auto mesh = meshes[index];
			auto& mesh_triangles = mesh->GetTriangles();
			auto size = static_cast<uint32_t>(triangles.size());
			triangles.insert(std::end(triangles), std::begin(mesh_triangles), std::end(mesh_triangles));
			root.triangle_indices.reserve(root.triangle_indices.size() + mesh_triangles.size());
			std::transform(std::begin(mesh_triangles), std::end(mesh_triangles), std::back_inserter(root.triangle_indices), [&](auto&) {
				triangles[size].mesh_index = index;
				return size++;
			});
		}

		UpdateTriangleBounds(node_index);
		SubdivideTriangles(node_index);
	}

	void Scene::UpdateTriangleBounds(uint32_t node_index) {
		BoundingBoxNode& node = bounding_volume_hierarchie[node_index];
		node.min_corner = glm::vec3(infinity);
		node.max_corner = glm::vec3(-infinity);

		for (const auto index : node.triangle_indices) {
			auto& triangle = triangles[index];

			for (auto const& vertex : triangle.vertices) {
				node.min_corner = glm::min(node.min_corner, snapToGrid(vertex.position));
				node.max_corner = glm::max(node.max_corner, snapToGrid(vertex.position));
			}
		}
	}

	
	void Scene::SubdivideTriangles(uint32_t node_index) {
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

		for (const auto index : node.triangle_indices) {
			auto& triangle = triangles[index];
			if (triangle.center[split_axis] < split_position) {
				left_child_triangle_indices.emplace_back(index);
			} else {
				right_child_triangle_indices.emplace_back(index);
			}
		}

		if (right_child_triangle_indices.empty() || left_child_triangle_indices.empty()) {
			return;
		}

		bounding_volume_hierarchie.reserve(bounding_volume_hierarchie.size() + 2);
		auto left_child_index = static_cast<uint32_t>(bounding_volume_hierarchie.size());
		auto& left_child = bounding_volume_hierarchie.emplace_back();
		auto right_child_index = static_cast<uint32_t>(bounding_volume_hierarchie.size());
		auto& right_child = bounding_volume_hierarchie.emplace_back();

		left_child.triangle_indices = std::move(left_child_triangle_indices);
		right_child.triangle_indices = std::move(right_child_triangle_indices);
		left_child.mesh_indices = node.mesh_indices;
		right_child.mesh_indices = std::move(node.mesh_indices);


		node.left_child_index = left_child_index;
		node.right_child_index = right_child_index;
		node.triangle_indices.clear();
		bounding_volume_hierarchie[node_index] = std::move(node);

		UpdateTriangleBounds(left_child_index);
		UpdateTriangleBounds(right_child_index);
		SubdivideTriangles(left_child_index);
		SubdivideTriangles(right_child_index);
	}
}
