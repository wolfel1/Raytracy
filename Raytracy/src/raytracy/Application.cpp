#include "raytracypch.h"

#include "Application.h"
#include "event/Event.h"
#include "renderer/Renderer.h"
#include "core/Timestep.h"
#include "renderer/model/Material.h"

#include <GLFW/glfw3.h>


namespace raytracy {
	IApplication* IApplication::instance = nullptr;

	Application::Application(const ApplicationSpecification& application_specification) : application_specification(application_specification) {
		RTY_PROFILE_FUNCTION();


		window = Window::Create({ application_specification.name, application_specification.width, application_specification.height });
		Renderer::Get().Init();


		auto& event_bus = EventBus::Get();
		event_bus.Register<WindowCloseEvent>(RTY_BIND_EVENT_FN(Application::OnWindowClose));
		event_bus.Register<WindowResizeEvent>(RTY_BIND_EVENT_FN(Application::OnWindowResize)); 
		event_bus.Register<AppTickEvent>(RTY_BIND_EVENT_FN(Application::OnAppTick));

		running = true;
	}

	Application::~Application() {
		RTY_PROFILE_FUNCTION();

		Shutdown();
	}

	void Application::Run() {
		RTY_PROFILE_FUNCTION();

		while (running) {
			RTY_PROFILE_SCOPE("RenderLoop"); 
			
			float time = (float)glfwGetTime();
			Timestep timestep = time - last_frame_time;
			last_frame_time = time;
			AppTickEvent e(timestep);
			EventBus::Get().Notify(e);

			if (!minimized) {
				for (auto& layer : layer_stack) {
					layer->OnUpdate(timestep);
				}
			}

			for (auto& layer : layer_stack) {
				layer->OnUIRender();
			}

			window->OnUpdate();
		}
	}

	void Application::Shutdown() {
		RTY_PROFILE_FUNCTION();

		for (auto& layer : layer_stack)
			layer->OnDetach();

		layer_stack.clear();
		window->Shutdown();
		Renderer::Get().Shutdown();
		ShaderLibrary::Get().Shutdown();
		renderer::MaterialLibrary::Get().Shutdown();

		running = false;
	}

	bool Application::OnWindowClose(Event& e) {
		running = false;

		return true;
	}

	bool Application::OnWindowResize(Event& e) {
		WindowResizeEvent evt = static_cast<WindowResizeEvent&>(e);

		if (evt.GetWidth() == 0 || evt.GetHeight() == 0) {
			minimized = true;
			return true;
		}
		application_specification.width = evt.GetWidth();
		application_specification.width = evt.GetHeight();
		minimized = false;

		return Renderer::Get().OnWindowResize(evt.GetWidth(), evt.GetHeight());
	}
	
	bool Application::OnAppTick(Event& e) {
		AppTickEvent evt = static_cast<AppTickEvent&>(e);

		std::stringstream ss;
		ss << window->GetName() << " @ " << evt.GetTimeStep().GetMilliseconds() << " ms per frame";
		window->SetTitle(ss.str());

		return true;
	}
}