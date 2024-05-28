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

	class IApplication {

	protected:
		static IApplication* instance;

	public:
		IApplication() {
			RTY_ASSERT(!instance, "App already exists!");
			instance = this;
		}
		virtual ~IApplication() {
			instance = nullptr;
		}
	
		static IApplication* Get() {
			return instance;
		}

		virtual shared_ptr<IWindow> GetWindow() const = 0;
		virtual ApplicationSpecification const& GetSpecification() const = 0;
		
	};

	class Application : public IApplication {
	private:
		ApplicationSpecification application_specification;
		shared_ptr<Window> window;
		bool running = false;
		bool minimized = false;
		float last_frame_time = 0.0f;

		std::vector<shared_ptr<Layer>> layer_stack;

	public:
		Application(const ApplicationSpecification& application_specification = ApplicationSpecification());
		Application(const Application&) = delete;
		virtual ~Application();

		void Run();

		void PushLayer(const shared_ptr<Layer>& layer) { layer_stack.emplace_back(layer); layer->OnAttach(); }

		void Shutdown();

		virtual shared_ptr<IWindow> GetWindow() const override {
			return window;
		}
		
		virtual ApplicationSpecification const& GetSpecification() const override {
			return application_specification;
		}

		private:
			bool OnWindowClose(Event& e); 
			bool OnWindowResize(Event& e);
			bool OnAppTick(Event& e);
	};

	Application* CreateApplication();
}

