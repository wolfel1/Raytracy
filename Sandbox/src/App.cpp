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
		auto& spec = IApplication::Get()->GetSpecification();
		camera_controller = make_unique<PerspectiveCameraController>(static_cast<float>(spec.width) / static_cast<float>(spec.height));

		scene = renderer::Scene::Create(camera_controller->GetCamera());


		auto ground = make_shared<renderer::Plane>(glm::vec3(0.0f, -1.5f, 0.0f), 10.0f);
		scene->AddMesh(ground);
		auto mesh = make_shared<renderer::Cube>(glm::vec3(0.0f, 0.0f, 0.0f));
		scene->AddMesh(mesh); 
		auto sphere = make_shared<renderer::Sphere>(glm::vec3(-1.5f, 0.0f, 0.0f));
		scene->AddMesh(sphere); 
		auto sphere1 = make_shared<renderer::Sphere>(glm::vec3(1.5f, 0.0f, 0.0f));
		scene->AddMesh(sphere1);

		auto green_material = renderer::MaterialLibrary::Get().Load("GreenMaterial");
		green_material->SetColor(glm::vec4(0.2f, 0.9f, 0.1f, 1.0f));
		sphere1->SetMaterial(green_material);

		auto red_material = renderer::MaterialLibrary::Get().Load("RedMaterial");
		red_material->SetColor(glm::vec4(0.9f, 0.1f, 0.1f, 1.0f));
		sphere->SetMaterial(red_material);

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
		scene->Destroy();
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