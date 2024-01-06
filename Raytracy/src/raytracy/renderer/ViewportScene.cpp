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
		scene_light = make_shared<DirectionalLight>();
		scene_light->color = {1.0f, 1.0f, 1.0f};
		scene_light->direction = {0.0f, 0.5f, 1.0f};
		scene_light->strength = 0.5f;
	}
}