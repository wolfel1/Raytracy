#include "raytracypch.h"

#include "Application.h"
#include "raytracer/Raytracer.h"
#include "event/Event.h"
#include "renderer/Renderer.h"

#include <GLFW/glfw3.h>

static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

namespace raytracy {
	Application* Application::instance = nullptr;

	Application::Application(const ApplicationSpecification& application_specification) : application_specification(application_specification) {
		RTY_PROFILE_FUNCTION();

		RTY_BASE_ASSERT(!instance, "App already exists!");

		instance = this;

		window = Window::Create({ application_specification.name, application_specification.width, application_specification.height });

		EventBus::Get().Register<WindowCloseEvent>(RTY_BIND_EVENT_FN(Application::OnWindowClose));

		Renderer::Get().Init();

		running = true;
	}

	Application::~Application() {
		RTY_PROFILE_FUNCTION();

		Shutdown();

		instance = nullptr;
	}

	void Application::Run() {
		RTY_PROFILE_FUNCTION();

		while (running) {
			for (auto& layer : layer_stack) {
				layer->OnUpdate();
			}

			for (auto& layer : layer_stack) {
				layer->OnUIRender();
			}

			window->OnUpdate();
		}
	}

	void Application::Shutdown() {
		Renderer::Get().Shutdown();

		for (auto& layer : layer_stack)
			layer->OnDetach();

		layer_stack.clear();

		running = false;
	}

	bool Application::OnWindowClose(Event& e) {
		running = false;

		return true;
	}
}