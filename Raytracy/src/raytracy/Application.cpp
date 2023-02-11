#include "raytracypch.h"

#include "Application.h"
#include "raytracer/Raytracer.h"

#include <GLFW/glfw3.h>

static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

namespace raytracy {
	Application* Application::instance = nullptr;

	Application::Application(const ApplicationSpecification& application_specification) : application_specification(application_specification) {
		RTY_PROFILE_FUNCTION();

		RTY_ASSERT(!instance, "App already exists!");

		instance = this;

		// Setup GLFW window
		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit())
		{
			std::cerr << "Could not initalize GLFW!\n";
			return;
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		window_handle = glfwCreateWindow(application_specification.width, application_specification.height, application_specification.name.c_str(), NULL, NULL);

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

		glfwDestroyWindow(window_handle);
		glfwTerminate();

		running = false;
	}
}