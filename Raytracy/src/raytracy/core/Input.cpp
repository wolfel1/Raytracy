#include "raytracypch.h"
#include "Input.h"

#include "raytracy/Application.h"

namespace raytracy {

	/*template<typename T>
	bool Input::IsKeyPressed(KeyCode key) const {
		auto window = static_cast<GLFWwindow*>(IApplication::Get()->GetWindow()->GetNativeWindow());
		auto state = T::GetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	template<typename T>
	bool Input::IsMouseButtonPressed(MouseCode button) const {
		auto window = static_cast<GLFWwindow*>(IApplication::Get()->GetWindow()->GetNativeWindow());
		auto state = T::GetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	template<typename T>
	glm::vec2 Input::GetMousePosition() const {
		auto window = static_cast<GLFWwindow*>(IApplication::Get()->GetWindow()->GetNativeWindow());
		double x, y;
		T::GetCursorPos(window, &x, &y);

		return { static_cast<float>(x), static_cast<float>(y) };
	}*/
}