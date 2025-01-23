#pragma once
#include "./camera/PerspectiveCamera.h"
#include "./model/Mesh.h"
#include "./model/DirectionalLight.h"
#include "raytracer/BoundingBoxNode.h"
#include <taskflow/taskflow.hpp>

namespace raytracy::renderer {

	class Scene {
	private:
		std::shared_ptr<PerspectiveCamera> camera;
		std::deque<std::shared_ptr<Mesh>> meshes;
		std::vector<Triangle> triangles;
		std::vector<BoundingBoxNode> bounding_volume_hierarchie;
		shared_ptr<Skybox> skybox;
		shared_ptr<DirectionalLight> light;

		static std::shared_ptr<Scene> instance;
	public:
		Scene(std::shared_ptr<PerspectiveCamera> camera);
		Scene(const Scene&) = delete;
		~Scene() = default;

		static std::shared_ptr<Scene> Create(std::shared_ptr<PerspectiveCamera> camera);

		static std::shared_ptr<Scene> Get() {
			return instance;
		}

		void Destroy() {
			instance = nullptr;
		}

		std::shared_ptr<PerspectiveCamera> GetCamera() const {
			return camera;
		}

		void AddMesh(std::shared_ptr<Mesh> const mesh);

		void AddSkybox();

		shared_ptr<Skybox> GetSkybox() const{
			return skybox;
		}

		std::deque<std::shared_ptr<Mesh>> const& GetMeshes() const {
			return meshes;
		}

		std::vector<Triangle> const& GetTriangles() const {
			return triangles;
		}
		
		std::vector<BoundingBoxNode> const& GetBoundingVolumeHierarchie() const {
			return bounding_volume_hierarchie;
		}

		std::shared_ptr<DirectionalLight> GetLight() const {
			return light;
		}

	private:

		void BuildBoundingVolumeHierarchie();
		void BuildTriangleBoundingVolumeHierarchie(uint32_t node_index);
		void UpdateMeshBounds(uint32_t node_index);
		void SubdivideMeshes(uint32_t node_index);
		void UpdateTriangleBounds(uint32_t node_index);
		void SubdivideTriangles(uint32_t node_index);
	};
}