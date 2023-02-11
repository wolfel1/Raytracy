#include "raytracypch.h"
#include "Window.h"

#include <GLFW/glfw3.h>

namespace raytracy {

	static bool is_glfw_initialized = false;

	static void GLFWErrorCallback(int error, const char* description) {
		RTY_BASE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	unique_ptr<Window> Window::Create(const WindowProps& props) {
		return make_unique<Window>(props);
	}

	Window::Window(const WindowProps& props) {
		window_data.name = props.name;
		window_data.width = props.width;
		window_data.height = props.height;

		RTY_BASE_INFO("Creating window {0} ({1}, {2})", props.name, props.width, props.height);
		if (!is_glfw_initialized) {
			auto success = glfwInit();
			RTY_BASE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			is_glfw_initialized = true;
		}

#ifdef RTY_DEBUG
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif
		window_handle = glfwCreateWindow(window_data.width, window_data.height, window_data.name.c_str(), NULL, NULL);

		//glfwMakeContextCurrent(window_handle);
		
		glfwSetWindowUserPointer(window_handle, &window_data);
		//SetVSync(true);
	}

	Window::~Window() {
		glfwDestroyWindow(window_handle);

		RTY_BASE_INFO("Terminate GLFW!");
		glfwTerminate();
	}

	void Window::OnUpdate() {
		glfwPollEvents();

		//glfwSwapBuffers(window_handle);
	}


	void Window::SetVSync(bool enabled) {
		if (enabled) {
			glfwSwapInterval(1);
		} else {
			glfwSwapInterval(0);
		}

		window_data.v_sync = enabled;
	}





}