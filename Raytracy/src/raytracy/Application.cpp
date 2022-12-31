#include "Application.h"

namespace raytracy {
	Application* Application::instance = nullptr;

	Application::Application() {
		instance = this;
		running = true;
	}

	Application::~Application() {
	}

	void Application::Run() {
		while (running) {

		}
	}
}