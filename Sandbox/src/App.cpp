#include <raytracy.h>
#include <raytracy/EntryPoint.h>

#include <iostream>

using namespace raytracy;

class SandboxLayer : public Layer {
private:
	std::shared_ptr<renderer::Scene> scene;

	unique_ptr<PerspectiveCameraController> camera_controller;

public:
	SandboxLayer() : Layer("SandboxLayer") {
	}

	void OnAttach() override {
		EventBus::Get().Register<KeyReleasedEvent>(RTY_BIND_EVENT_FN(SandboxLayer::OnKeyReleased));
		auto spec = Application::Get().GetSpecification();
		camera_controller = make_unique<PerspectiveCameraController>(static_cast<float>(spec.width) / static_cast<float>(spec.height));
		camera_controller->Translate({ 0.0f, 5.0f, 10.0f });
		camera_controller->RotateX(-20.0f);

		renderer::Scene::Create(camera_controller->GetCamera());
		scene = renderer::Scene::Get();

		auto mesh = make_shared<renderer::Plane>();
		scene->AddMesh(mesh);
		/*for (int a = 0; a < 100; a++) {
			glm::vec3 center(Random::RandomFloat(-5.0f, 5.0f), Random::RandomFloat(-5.0f, 5.0f), Random::RandomFloat(-5.0f, 5.0f));

			auto color = glm::vec4(Random::RandomVector(0.0f, 1.0f), 1.0f);
			shared_ptr<renderer::Material>sphere_material = std::make_shared<renderer::Material>(color);
			auto mesh = make_shared<renderer::Sphere>(center, Random::RandomFloat(0.1f, 1.0f));
			mesh->SetMaterial(sphere_material);
			scene->AddMesh(mesh);
		}*/

	}

	void OnUpdate(Timestep timestep) override {
		camera_controller->OnUpdate(timestep);

		Renderer::Get().Submit(scene);
	}

	void RaytraceScene() {
			Raytracer raytracer;
			raytracer.Submit();
	}

	bool OnKeyReleased(Event& e) {
		KeyReleasedEvent evt = static_cast<KeyReleasedEvent&>(e);
		if (evt.GetKeyCode() == KeyCode::Escape) {
			WindowCloseEvent e;
			EventBus::Get().Notify(e);
		} else if (evt.GetKeyCode() == KeyCode::R) {
			RaytraceScene();
		}
		return false;
	}
	
	void OnDetach() override {
		renderer::Scene::Destroy();
	};

	~SandboxLayer() = default;

};

raytracy::Application* raytracy::CreateApplication() {
	ApplicationSpecification spec;
	spec.name = "Sandbox";
	spec.width = 1000;
	spec.height = 700;

	Application* app = new Application(spec);

	app->PushLayer(make_shared<SandboxLayer>());

	return app;
}