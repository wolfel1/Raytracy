#include "raytracypch.h"

#include "Application.h"
#include "raytracer/Raytracer.h"

namespace raytracy {
	Application* Application::instance = nullptr;

	Application::Application(const ApplicationSpecification& application_specification) : application_specification(application_specification) {
		RTY_PROFILE_FUNCTION();

		RTY_ASSERT(!instance, "App already exists!");

		instance = this;
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
			for (auto& layer : layer_stack)
				layer->OnUpdate();

			for (auto& layer : layer_stack)
				layer->OnUIRender();
		}
	}

	void Application::Shutdown() {
		for (auto& layer : layer_stack)
			layer->OnDetach();

		layer_stack.clear();

		running = true;
	}
}