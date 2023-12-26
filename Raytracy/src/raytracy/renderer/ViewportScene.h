#pragma once
#include "./camera/PerspectiveCamera.h"

namespace raytracy::renderer {

	struct DirectionalLight {
		glm::vec3 color;
		glm::vec3 direction;
		float strength;

		shared_ptr<UniformBuffer> light_uniform_buffer;
	};

	class Scene {
	private:
		std::shared_ptr<PerspectiveCamera> camera;

		DirectionalLight scene_light{};

		static std::shared_ptr<Scene> instance;
	public:
		Scene(const Scene&) = delete;

		static void Create(std::shared_ptr<PerspectiveCamera> camera);

		static std::shared_ptr<Scene> Get() {
			return instance;
		}

		std::shared_ptr<PerspectiveCamera> GetCamera() {
			return camera;
		}

		DirectionalLight& GetSceneLight() {
			return scene_light;
		}

	private:
		Scene(std::shared_ptr<PerspectiveCamera> camera);
	};
}