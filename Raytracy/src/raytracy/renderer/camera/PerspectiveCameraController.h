#pragma once

#include "PerspectiveCamera.h"
#include "raytracy/core/Timestep.h"

#include "raytracy/event/ApplicationEvent.h"
#include "raytracy/event/MouseEvent.h"

namespace raytracy {

	class PerspectiveCameraController {

	private:
		float aspect_ratio;
		float field_of_view = 45.0f;
		std::shared_ptr<PerspectiveCamera> camera;

		float translation_speed = 0.08f;
		float scroll_speed = 0.5f;
		float rotation_speed = .2f;
		float orbit_speed = 180.0f;

		glm::vec2 last_mouse_position{};
		bool is_not_rotating = true;

	public:
		PerspectiveCameraController(float aspect_ratio);

		void OnUpdate(Timestep timestep);

		std::shared_ptr<PerspectiveCamera> GetCamera() const {
			return camera;
		}

		float GetFieldOfView() const {
			return field_of_view;
		}
		
		float GetAspectRatio() const {
			return aspect_ratio;
		}

		void Translate(glm::vec3 const& amount);
		void TranslateX(float const amount);
		void TranslateY(float const amount);
		void TranslateZ(float const amount);

		void RotateX(float const degree);
		void RotateY(float const degree);

	private:
		bool OnWindowResized(Event& e);
		bool OnMouseScrolled(Event& e);
	};
}