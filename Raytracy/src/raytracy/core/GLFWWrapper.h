#pragma once

#include <GLFW/glfw3.h>

namespace raytracy {

	/*class IGLFWWrapper {
	public:
		virtual int GetKey(GLFWwindow* window, int key) = 0;
		virtual int GetMouseButton(GLFWwindow* window, int button) = 0;
		virtual void GetCursorPos(GLFWwindow* window, double* x, double* y) = 0;
	};*/

	class GLFWWrapper {
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