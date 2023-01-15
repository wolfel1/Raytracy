#include "raytracypch.h"

#include "Application.h"
#include "raytracer/Raytracer.h"
#include "raytracer/shapes/Sphere.h"

namespace raytracy {
	Application* Application::instance = nullptr;

	Application::Application() {
		RTY_PROFILE_FUNCTION();

		RTY_ASSERT(!instance, "App already exists!");

		instance = this;
		running = true;

		Raytracer::Init();
	}

	Application::~Application() {
		RTY_PROFILE_FUNCTION();
		Raytracer::Shutdown();
	}

	void Application::Run() {
		RTY_PROFILE_FUNCTION();

		std::cin.get();
		/*while (running) {

		}*/
	}
}