#pragma once

#include <GLFW/glfw3.h>

namespace raytracy {


	class GLFWProxy {
	public:
		static int GetKey(GLFWwindow* window, int key) {
			return glfwGetKey(window, key);
		}

		static int GetMouseButton(GLFWwindow* window, int button) {
			return glfwGetMouseButton(window, button);
		}

		static void GetCursorPos(GLFWwindow* window, double* x, double* y) {
			glfwGetCursorPos(window, x, y);
		}

	};
}