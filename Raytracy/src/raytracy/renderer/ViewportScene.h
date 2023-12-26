#pragma once
#include "./camera/PerspectiveCamera.h"

namespace raytracy::renderer {
	class Scene {
	private:
		std::shared_ptr<PerspectiveCamera> camera;

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

	private:
		Scene(std::shared_ptr<PerspectiveCamera> camera) : camera(camera) {}
	};
}