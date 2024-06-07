#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"
#include <glm/glm.hpp>
#include "GLFWProxy.h"
#include "raytracy/Application.h"

namespace raytracy {

	class Input {
	private:

	public:
		Input(const Input&) = delete;

		template<typename T = GLFWProxy>
		bool IsKeyPressed(KeyCode key) const {
			auto window = static_cast<GLFWwindow*>(IApplication::Get()->GetWindow()->GetNativeWindow());
			auto state = T::GetKey(window, static_cast<int32_t>(key));
			return state == GLFW_PRESS || state == GLFW_REPEAT;
		}

		template<typename T = GLFWProxy>
		bool IsMouseButtonPressed(MouseCode button) const {
			auto window = static_cast<GLFWwindow*>(IApplication::Get()->GetWindow()->GetNativeWindow());
			auto state = T::GetMouseButton(window, static_cast<int32_t>(button));
			return state == GLFW_PRESS || state == GLFW_REPEAT;
		}

		template<typename T = GLFWProxy>
		glm::vec2 GetMousePosition() const {
			auto window = static_cast<GLFWwindow*>(IApplication::Get()->GetWindow()->GetNativeWindow());
			double x, y;
			T::GetCursorPos(window, &x, &y);

			return { static_cast<float>(x), static_cast<float>(y) };
		}

		static Input& Get() {
			static Input instance;
			return instance;
		}

	private:
		Input() {}
	};
}
