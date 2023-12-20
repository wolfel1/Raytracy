#include <raytracy.h>
#include <raytracy/EntryPoint.h>

#include <iostream>

using namespace raytracy;

class SandboxLayer : public Layer {
private:
	shared_ptr<renderer::Plane> ground;
	shared_ptr<renderer::Cube> cube1;
	shared_ptr<renderer::Cube> cube2;

	unique_ptr<PerspectiveCameraController> camera;

public:
	SandboxLayer() : Layer("SandboxLayer") {
	}

	void OnAttach() override {
		EventBus::Get().Register<KeyReleasedEvent>(RTY_BIND_EVENT_FN(SandboxLayer::OnKeyReleased));
		camera = make_unique<PerspectiveCameraController>(1000.0f / 700.0f);
		camera->Translate({0.0f, 2.0f, 5.0f});
		camera->RotateX(-20.0f);

		ground = make_shared<renderer::Plane>(glm::vec3(0.0f, -0.5f, 0.0f), 10.0f);
		ground->SetDisplayColor({0.8f, 0.8f, 0.0f, 1.0f});
		cube1 = make_shared<renderer::Cube>(glm::vec3(-1.0f, 0.0f, 0.0f));
		cube2 = make_shared<renderer::Cube>(glm::vec3(1.0f, 0.0f, 0.0f));
		cube2->SetDisplayColor({1.0f,0.0f,0.0f,1.0f});
	}

	void OnUpdate(Timestep timestep) override {
		camera->OnUpdate(timestep);

		auto& renderer = Renderer::Get();

		renderer.BeginScene(camera->GetCamera());
		renderer.Submit(ground);
		renderer.Submit(cube1);
		renderer.Submit(cube2);
		renderer.EndScene();
	}

	void RaytraceScene() {
		const auto aspect_ratio = 16.0f / 9.0f;

		uint32_t width = 480;
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
			auto material_left = make_shared<Dielectric>(1.5f);
			auto material_right = make_shared<Metal>(glm::vec4(0.8f, 0.6f, 0.2f, 1.0f), 0.0f);

			scene.Add(make_shared<raytracer::Plane>(glm::vec3(0.0f, -1.0f, -1.0f), 5.0f,
										  material_ground));

			glm::vec3 look_from(0, 2, 7);
			glm::vec3 look_at(0, 0, -1);
			glm::vec3 up(0, 1, 0);

			Camera camera(look_from, look_at, up, 20.0f, aspect_ratio);

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