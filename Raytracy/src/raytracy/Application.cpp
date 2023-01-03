#include "raytracypch.h"

#include "Application.h"
#include "raytracer/Raytracer.h"

namespace raytracy {
	Application* Application::instance = nullptr;

	Application::Application() {
		instance = this;
		running = true;

		Raytracer::Init();
	}

	Application::~Application() {
	}

	void Application::Run() {
		while (running) {

		}
	}
}