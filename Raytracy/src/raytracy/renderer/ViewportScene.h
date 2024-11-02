#pragma once
#include "./camera/PerspectiveCamera.h"
#include "./model/Mesh.h"
#include "raytracer/BoundingBoxNode.h"

namespace raytracy::renderer {

	struct DirectionalLight {
		glm::vec3 color;
		glm::vec3 direction;
		float strength;

		shared_ptr<OpenGLUniformBuffer> light_uniform_buffer;

	};

	class Scene {
	private:
		std::shared_ptr<PerspectiveCamera> camera;
		std::deque<std::shared_ptr<Mesh>> meshes;
		std::vector<shared_ptr<Triangle>> triangles;
		std::vector<BoundingBoxNode> bounding_volume_hierarchie;
		shared_ptr<Skybox> skybox;

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

	private:

		void BuildBoundingVolumeHierarchie();
		void UpdateBounds(uint32_t node_index);
		void Subdivide(uint32_t node_index);
		void BuildLeafs(uint32_t node_index);
		void AddTriangles(uint32_t node_index);
	};
}