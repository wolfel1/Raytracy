#include <raytracy.h>
#include <raytracy/EntryPoint.h>

#include <iostream>

using namespace raytracy;

class SandboxLayer : public Layer {
private:
	std::shared_ptr<renderer::Scene> scene;
	std::shared_ptr<renderer::MeshMaterial> ground_material;

	unique_ptr<PerspectiveCameraController> camera_controller;


public:
	SandboxLayer() : Layer("SandboxLayer") {
	}

	void OnAttach() override {
		EventBus::Get().Register<KeyReleasedEvent>(RTY_BIND_EVENT_FN(SandboxLayer::OnKeyReleased));
		auto spec = Application::Get().GetSpecification();
		camera_controller = make_unique<PerspectiveCameraController>(static_cast<float>(spec.width) / static_cast<float>(spec.height));
		camera_controller->Translate({ 0.0f, 5.0f, 10.0f });
		camera_controller->RotateX(-25.0f);

		renderer::Scene::Create(camera_controller->GetCamera());
		scene = renderer::Scene::Get();

		auto sphere = make_shared<renderer::Sphere>(glm::vec3(0.0f, 0.0f, 0.0f));
		scene->AddMesh(sphere); 
		auto ground = make_shared<renderer::Sphere>(glm::vec3(0.0f, -22.0f, 0.0f), 20.0f);
		ground_material = make_shared<renderer::MeshMaterial>(glm::vec4(0.1f, 0.8f, 0.0f, 1.0f));
		ground->SetMaterial(ground_material);
		scene->AddMesh(ground);

		scene->AddSkybox();
	}

	void OnUpdate(Timestep timestep) override {
		camera_controller->OnUpdate(timestep);

		Renderer::Get().Submit(scene);
		
	}


	bool OnKeyReleased(Event& e) {
		KeyReleasedEvent evt = static_cast<KeyReleasedEvent&>(e);
		if (evt.GetKeyCode() == KeyCode::Escape) {
			WindowCloseEvent e;
			EventBus::Get().Notify(e);
		} else if (evt.GetKeyCode() == KeyCode::R) {
			Renderer::Get().ToggleRaytracing();
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