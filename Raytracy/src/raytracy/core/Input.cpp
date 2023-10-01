#include "raytracypch.h"
#include "Input.h"

#include "raytracy/Application.h"
#include <GLFW/glfw3.h>

namespace raytracy {
	bool Input::IsKeyPressed(KeyCode key) const {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool Input::IsMouseButtonPressed(MouseCode button) const {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	glm::vec2 Input::GetMousePosition() const {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		return { static_cast<float>(x), static_cast<float>(y) };
	}
}