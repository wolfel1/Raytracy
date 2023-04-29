#include "raytracypch.h"

#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>

namespace raytracy {
	OpenGLContext::OpenGLContext(GLFWwindow* window_handle) : window_handle(window_handle) {
		RTY_ASSERT(window_handle, "Window handle is null!");
	}

	void OpenGLContext::Init() {
		glfwMakeContextCurrent(window_handle);

		RTY_PROFILE_SCOPE("LoadGlad");
		int status = gladLoadGL(glfwGetProcAddress);
		RTY_ASSERT(status, "Failed to initialize OpenGL!");
		RTY_BASE_INFO("OpenGL Info:");
		RTY_BASE_INFO("  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		RTY_BASE_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		RTY_BASE_INFO("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
		RTY_BASE_TRACE("Loaded OpenGL version {0}.{1}\n", GLAD_VERSION_MAJOR(status), GLAD_VERSION_MINOR(status));
	}
}