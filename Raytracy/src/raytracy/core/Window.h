#pragma once

#include <string>

#include "Core.h"
#include "../renderer/api/GraphicsContext.h"

struct GLFWwindow;

namespace raytracy {

	class Window {
	private:
		struct WindowData {
			std::string name;
			uint32_t width;
			uint32_t height;
			bool v_sync;
		};
		GLFWwindow* window_handle = nullptr;

		WindowData window_data;

		shared_ptr<GraphicsContext> graphics_context;

	public:

		static bool is_glfw_initialized;

		struct WindowProps {
			std::string name;
			uint32_t width;
			uint32_t height;
		};
		Window(const WindowProps& props);
		~Window();

		void OnUpdate();

		uint32_t GetWidth() const { return window_data.width; }
		uint32_t GetHeight() const { return window_data.height; }

		void SetTitle(const std::string& name);
		std::string GetName() const { return window_data.name; }

		shared_ptr<GraphicsContext> GetGraphicsContext() const {
			return graphics_context;
		}

		void SetVSync(bool enabled);
		bool IsVSync() const { return window_data.v_sync; }

		void* GetNativeWindow() const {
			return window_handle;
		}

		void Shutdown();

		static shared_ptr<Window> Create(const WindowProps& props) {
			return make_shared<Window>(props);
		}
	};
}