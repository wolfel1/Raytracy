#pragma once

#include <string>

#include "Core.h"
#include "../renderer/api/opengl/OpenGLContext.h"

struct GLFWwindow;

namespace raytracy {

	struct WindowData {
		std::string name;
		uint32_t width;
		uint32_t height;
		bool v_sync;
	};

	struct WindowProps {
		std::string name;
		uint32_t width;
		uint32_t height;
	};

	class IWindow {
		public:
			virtual void* GetNativeWindow() const = 0;

			virtual uint32_t GetWidth() const = 0;
			virtual uint32_t GetHeight() const = 0;
	};

	class Window : public IWindow {
	private:
		GLFWwindow* window_handle = nullptr;

		WindowData window_data;

		shared_ptr<OpenGLContext> graphics_context;

	public:

		static bool is_glfw_initialized;

		Window(const WindowProps& props);
		~Window();

		void OnUpdate();

		virtual uint32_t GetWidth() const override { return window_data.width; }
		virtual uint32_t GetHeight() const override { return window_data.height; }

		void SetTitle(const std::string& name);
		std::string GetName() const { return window_data.name; }

		void SwapBuffers();
		void SetVSync(bool enabled);
		bool IsVSync() const { return window_data.v_sync; }

		virtual void* GetNativeWindow() const override {
			return window_handle;
		}

		void Shutdown();

		static shared_ptr<Window> Create(const WindowProps& props) {
			return make_shared<Window>(props);
		}
	};
}