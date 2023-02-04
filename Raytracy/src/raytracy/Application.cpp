#include "raytracypch.h"

#include "Application.h"
#include "raytracer/Raytracer.h"

namespace raytracy {
	Application* Application::instance = nullptr;

	Application::Application() {
		RTY_PROFILE_FUNCTION();

		RTY_ASSERT(!instance, "App already exists!");

		instance = this;
		running = true;
	}

	Application::~Application() {
		RTY_PROFILE_FUNCTION();
	}

	void Application::Run() {
		RTY_PROFILE_FUNCTION();

		std::cin.get();
		/*while (running) {

		}*/
	}
}