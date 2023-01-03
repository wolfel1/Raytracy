#include "raytracypch.h"

#include "Application.h"
#include "raytracer/Raytracer.h"

namespace raytracy {
	Application* Application::instance = nullptr;

	Application::Application() {
		RTY_ASSERT(!instance, "App already exists!");

		instance = this;
		running = true;

		Raytracer::Init();
	}

	Application::~Application() {
		Raytracer::Shutdown();
	}

	void Application::Run() {
		while (running) {

		}
	}
}