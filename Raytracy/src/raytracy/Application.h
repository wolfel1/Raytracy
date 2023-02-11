#pragma once

#include <vector>

#include "core/Layer.h"
#include "core/Core.h"

struct GLFWwindow;

namespace raytracy {
	struct ApplicationSpecification {
		std::string name = "Raytracy App";
		uint32_t width = 1600;
		uint32_t height = 900;
	};

	class Application {
	private:
		ApplicationSpecification application_specification;
		GLFWwindow* window_handle = nullptr;
		bool running = false;

		std::vector<shared_ptr<Layer>> layer_stack;

		static Application* instance;
	public:
		Application(const ApplicationSpecification& application_specification = ApplicationSpecification());
		Application(const Application&) = delete;
		~Application();

		void Run();
		void PushLayer(const shared_ptr<Layer>& layer) { layer_stack.emplace_back(layer); layer->OnAttach(); }

		void Shutdown();

		static Application& Get() {
			return *instance;
		}
	};

	Application* CreateApplication();
}

