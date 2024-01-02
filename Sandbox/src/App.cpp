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
		camera_controller->Translate({ 0.0f, 5.0f, 20.0f });
		camera_controller->RotateX(-20.0f);

		renderer::Scene::Create(camera_controller->GetCamera());
		scene = renderer::Scene::Get();

		for (int a = 0; a < 100; a++) {
			glm::vec3 center(Random::RandomFloat(-5.0f, 5.0f), Random::RandomFloat(-5.0f, 5.0f), Random::RandomFloat(-5.0f, 5.0f));

			auto color = glm::vec4(Random::RandomVector(0.0f, 1.0f), 1.0f);
			shared_ptr<renderer::Material>sphere_material = std::make_shared<renderer::Material>(color);
			auto mesh = make_shared<renderer::Sphere>(center, Random::RandomFloat(0.1f, 1.0f));
			mesh->SetMaterial(sphere_material);
			scene->AddMesh(mesh);
		}

	}

	void OnUpdate(Timestep timestep) override {
		camera_controller->OnUpdate(timestep);

		Renderer::Get().Submit(scene);
	}

	void RaytraceScene() {
		const auto aspect_ratio = camera_controller->GetAspectRatio();

		uint32_t width = 720;
		uint32_t height = static_cast<uint32_t>(width / aspect_ratio);
		uint32_t samples_per_pixel = 50;
		uint32_t max_depth = 50;
		shared_ptr<Image> image = make_shared<Image>(width, height, samples_per_pixel, max_depth);

		{
			raytracer::Scene scene;

			for (auto mesh : this->scene->GetMeshes()) {

				auto material =
					make_shared<LambertianDiffuse>(mesh->GetMaterial()->GetColor());

				scene.Add(make_shared<raytracer::Sphere>(mesh->GetOrigin(), mesh->GetScale(),
														 material));
			}

			auto viewport_camera = camera_controller->GetCamera();
			glm::vec3 look_from = viewport_camera->GetPosition();
			glm::vec3 look_at = viewport_camera->GetDirection();
			glm::vec3 up = viewport_camera->GetUp();

			Camera camera(look_from, look_at, up, camera_controller->GetFieldOfView(), aspect_ratio);

			Raytracer raytracer;
			raytracer.Submit(scene, camera, image);
		}

		image->WriteImage();
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

	Raytracer r;
	r.RaytraceScene();

	//app->PushLayer(make_shared<SandboxLayer>());

	return app;
}