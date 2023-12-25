#include <raytracy.h>
#include <raytracy/EntryPoint.h>

#include <iostream>

using namespace raytracy;

class SandboxLayer : public Layer {
private:
	shared_ptr<renderer::Plane> ground;
	shared_ptr<renderer::Sphere> sphere;

	unique_ptr<PerspectiveCameraController> camera_controller;

public:
	SandboxLayer() : Layer("SandboxLayer") {
	}

	void OnAttach() override {
		EventBus::Get().Register<KeyReleasedEvent>(RTY_BIND_EVENT_FN(SandboxLayer::OnKeyReleased));
		camera_controller = make_unique<PerspectiveCameraController>(1000.0f / 700.0f);
		camera_controller->Translate({ 0.0f, 2.0f, 8.0f });
		camera_controller->RotateX(-20.0f);

		ground = make_shared<renderer::Plane>(glm::vec3(0.0f, -1.5f, 0.0f), 10.0f);
		ground->SetDisplayColor({ 0.8f, 0.8f, 0.0f, 1.0f });
		sphere = make_shared<renderer::Sphere>(glm::vec3(0.0f, 0.0f, 0.0f));
	}

	void OnUpdate(Timestep timestep) override {
		camera_controller->OnUpdate(timestep);

		auto& renderer = Renderer::Get();

		renderer.BeginScene(camera_controller->GetCamera());
		renderer.Submit(ground);
		renderer.Submit(sphere);
		renderer.EndScene();
	}

	void RaytraceScene() {
		const auto aspect_ratio = 16.0f / 9.0f;

		uint32_t width = 1000 / 2;
		uint32_t height = static_cast<int>(width / aspect_ratio);
		uint32_t samples_per_pixel = 100;
		uint32_t max_depth = 50;
		shared_ptr<Image> image = make_shared<Image>(width, height, samples_per_pixel, max_depth);

		{
			Scene scene;

			auto material_ground =
				make_shared<LambertianDiffuse>(glm::vec4(0.8f, 0.8f, 0.0f, 1.0f));
			auto material_center =
				make_shared<LambertianDiffuse>(glm::vec4(0.7f, 0.3f, 0.3f, 1.0f));

			scene.Add(make_shared<raytracer::Plane>(ground->GetOrigin(), ground->GetScale(),
													material_ground));
			scene.Add(make_shared<raytracer::Sphere>(sphere->GetOrigin(), sphere->GetScale(),
													 material_center));

			auto& viewport_camera = camera_controller->GetCamera();
			glm::vec3 look_from = viewport_camera.GetPosition();
			glm::vec3 look_at = viewport_camera.GetDirection();
			glm::vec3 up = viewport_camera.GetUp();

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
	~SandboxLayer() {}

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