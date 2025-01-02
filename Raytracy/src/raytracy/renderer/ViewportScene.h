#pragma once
#include "./camera/PerspectiveCamera.h"
#include "./model/Mesh.h"
#include "./model/DirectionalLight.h"
#include "raytracer/BoundingBoxNode.h"

namespace raytracy::renderer {

	class Scene {
	private:
		std::shared_ptr<PerspectiveCamera> camera;
		std::deque<std::shared_ptr<Mesh>> meshes;
		std::vector<shared_ptr<Triangle>> triangles;
		std::vector<BoundingBoxNode> bounding_volume_hierarchie;
		shared_ptr<Skybox> skybox;
		shared_ptr<DirectionalLight> light;

		static std::shared_ptr<Scene> instance;
	public:
		Scene(std::shared_ptr<PerspectiveCamera> camera);
		Scene(const Scene&) = delete;
		~Scene() = default;

		static void Create(std::shared_ptr<PerspectiveCamera> camera);

		static std::shared_ptr<Scene> Get() {
			return instance;
		}

		static void Destroy() {
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

		std::vector<std::shared_ptr<Triangle>> const& GetTriangles() const {
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
		void UpdateBounds(uint32_t node_index);
		void Subdivide(uint32_t node_index);
		void AddTriangles(std::shared_ptr<Mesh> const mesh);
		BoundingBox GetTriangleBoundingBox(Triangle& triangle);
	};
}