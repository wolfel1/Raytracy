#pragma once

struct GLFWwindow;

namespace raytracy {
	class OpenGLContext {

	private:
		GLFWwindow* window_handle;

	public:
		OpenGLContext(GLFWwindow* window_handle);

		void Init();

	};
}