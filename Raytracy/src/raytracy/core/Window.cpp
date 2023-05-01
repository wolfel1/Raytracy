#include "raytracypch.h"
#include "Window.h"

#include "../event/ApplicationEvent.h"
#include "../event/KeyEvent.h"
#include "../renderer/api/RendererAPI.h"

#include <GLFW/glfw3.h>

namespace raytracy {

	bool Window::is_glfw_initialized = false;

	static void GLFWErrorCallback(int error, const char* description) {
		RTY_BASE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window::Window(const WindowProps& props) {
		RTY_PROFILE_FUNCTION();
		window_data.name = props.name;
		window_data.width = props.width;
		window_data.height = props.height;

		RTY_BASE_TRACE("Creating window {0} ({1}, {2})", props.name, props.width, props.height);
		if (!is_glfw_initialized) {
			RTY_PROFILE_SCOPE("GLFWInit");
			auto success = glfwInit();
			RTY_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			is_glfw_initialized = true;
		}

		{
			RTY_PROFILE_SCOPE("CreateWindow");
#ifdef RTY_DEBUG
			if (RendererAPI::GetAPI() == RendererAPI::API::OpenGL) {
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
			} else if (RendererAPI::GetAPI() == RendererAPI::API::Vulkan) {
				glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			}
#endif
			glfwWindowHint(GLFW_SAMPLES, 4);
			window_handle = glfwCreateWindow(window_data.width, window_data.height, window_data.name.c_str(), NULL, NULL);
			RTY_ASSERT(window_handle, "Could not create window!");
		}

		graphics_context = GraphicsContext::Create(window_handle);
		graphics_context->Init();

		glfwSetWindowUserPointer(window_handle, &window_data);
		SetVSync(true);


		{
			RTY_PROFILE_SCOPE("SetEventCallbacks");
			glfwSetWindowCloseCallback(window_handle, [](GLFWwindow* window) {
				WindowCloseEvent e;
				EventBus::Get().Notify(e);
			});

			glfwSetKeyCallback(window_handle, [](GLFWwindow* window, int key, int scancode, int action, int mods) {

				switch (action) {
				case GLFW_PRESS:
				{
					KeyPressedEvent e(static_cast<KeyCode>(key), 0);
					EventBus::Get().Notify(e);
					break;
				}

				case GLFW_RELEASE:
				{
					KeyReleasedEvent e(static_cast<KeyCode>(key));
					EventBus::Get().Notify(e);
					break;
				}

				case GLFW_REPEAT:
				{
					KeyPressedEvent e(static_cast<KeyCode>(key), 1);
					EventBus::Get().Notify(e);
					break;
				}
				}
			});

			glfwSetWindowSizeCallback(window_handle, [](GLFWwindow* window, int width, int height) {
				WindowResizeEvent e(width, height);
				EventBus::Get().Notify(e);
			});
		}
	}

	Window::~Window() {
		RTY_PROFILE_FUNCTION();
		glfwDestroyWindow(window_handle);

		RTY_BASE_INFO("Terminate GLFW!");
		glfwTerminate();
	}

	void Window::OnUpdate() {
		RTY_PROFILE_FUNCTION();

		graphics_context->SwapBuffers();
		glfwPollEvents();
	}


	void Window::SetTitle(const std::string& name) {
		glfwSetWindowTitle(window_handle, name.c_str());
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