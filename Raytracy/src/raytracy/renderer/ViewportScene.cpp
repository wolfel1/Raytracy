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
}