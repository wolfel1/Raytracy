#pragma once

#include <vector>

#include "core/Layer.h"
#include "core/Core.h"
#include "core/Window.h"
#include "event/ApplicationEvent.h"



namespace raytracy {
	struct ApplicationSpecification {
		std::string name = "Raytracy App";
		uint32_t width = 1600;
		uint32_t height = 900;
	};

	class Application {
	private:
		ApplicationSpecification application_specification;
		shared_ptr<Window> window;
		bool running = false;
		bool minimized = false;
		float last_frame_time = 0.0f;

		std::vector<shared_ptr<Layer>> layer_stack;

		static Application* instance;
	public:
		Application(const ApplicationSpecification& application_specification = ApplicationSpecification());
		Application(const Application&) = delete;
		~Application();

		void Run();

		void PushLayer(const shared_ptr<Layer>& layer) { layer_stack.emplace_back(layer); layer->OnAttach(); }

		void Shutdown();

		shared_ptr<Window> GetWindow() const {
			return window;
		}
		
		ApplicationSpecification const& GetSpecification() const {
			return application_specification;
		}

		static Application& Get() {
			return *instance;
		}

		private:
			bool OnWindowClose(Event& e); 
			bool OnWindowResize(Event& e);
			bool OnAppTick(Event& e);
	};

	Application* CreateApplication();
}

