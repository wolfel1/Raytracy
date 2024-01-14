#pragma once
#include "./camera/PerspectiveCamera.h"
#include "./model/Mesh.h"

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
		shared_ptr<Skybox> skybox;

		shared_ptr<DirectionalLight> scene_light;

		static std::shared_ptr<Scene> instance;
	public:
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

		shared_ptr<DirectionalLight> GetSceneLight() const {
			return scene_light;
		}

		void AddMesh(std::shared_ptr<Mesh> const mesh) {
			meshes.push_back(mesh);
		}

		void AddSkybox();

		shared_ptr<Skybox> GetSkybox() const{
			return skybox;
		}

		std::deque<std::shared_ptr<Mesh>> const& GetMeshes() const {
			return meshes;
		}

	private:
		Scene(std::shared_ptr<PerspectiveCamera> camera);
	};
}