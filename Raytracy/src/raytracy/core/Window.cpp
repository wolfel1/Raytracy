#include "raytracypch.h"
#include "Window.h"

#include "../event/ApplicationEvent.h"
#include "../event/KeyEvent.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>



namespace raytracy {

	static bool is_glfw_initialized = false;

	static void GLFWErrorCallback(int error, const char* description) {
		RTY_BASE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	unique_ptr<Window> Window::Create(const WindowProps& props) {
		return make_unique<Window>(props);
	}

	Window::Window(const WindowProps& props) {
		RTY_PROFILE_FUNCTION();
		window_data.name = props.name;
		window_data.width = props.width;
		window_data.height = props.height;



		RTY_BASE_INFO("Creating window {0} ({1}, {2})", props.name, props.width, props.height);
		if (!is_glfw_initialized) {
			RTY_PROFILE_SCOPE("GLFWInit");
			auto success = glfwInit();
			RTY_BASE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			is_glfw_initialized = true;
		}

		{
#ifdef RTY_DEBUG
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
			glfwWindowHint(GLFW_SAMPLES, 4);

			RTY_PROFILE_SCOPE("CreateWindow");
			window_handle = glfwCreateWindow(window_data.width, window_data.height, window_data.name.c_str(), NULL, NULL);
			RTY_BASE_ASSERT(window_handle, "Could not create window!");
		}

		glfwMakeContextCurrent(window_handle);
		{
			RTY_PROFILE_SCOPE("LoadGlad");
			int status = gladLoadGL(glfwGetProcAddress);
			RTY_BASE_ASSERT(status, "Failed to initialize OpenGL!");
			RTY_BASE_INFO("OpenGL Info:");
			RTY_BASE_INFO("  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
			RTY_BASE_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
			RTY_BASE_INFO("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
			RTY_BASE_TRACE("Loaded OpenGL version {0}.{1}\n", GLAD_VERSION_MAJOR(status), GLAD_VERSION_MINOR(status));
		}
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

		glfwSwapBuffers(window_handle);
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