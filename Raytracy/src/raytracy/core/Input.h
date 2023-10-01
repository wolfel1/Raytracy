#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"
#include <glm/glm.hpp>

namespace raytracy {

	class Input {

	public:
		Input(const Input&) = delete;

		bool IsKeyPressed(KeyCode key) const;

		bool IsMouseButtonPressed(MouseCode button) const;

		glm::vec2 GetMousePosition() const;

		static Input& Get() {
			static Input instance;
			return instance;
		}

	private:
		Input() {}
	};
}
