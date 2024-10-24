#pragma once

#include "PerspectiveCamera.h"
#include "raytracy/core/Timestep.h"

#include "raytracy/event/Event.h"

namespace raytracy {

	class PerspectiveCameraController {

	private:
		float aspect_ratio;
		float field_of_view = 45.0f;
		std::shared_ptr<PerspectiveCamera> camera;

		float translation_speed = 0.08f;
		float scroll_speed = 5.f;
		float rotation_speed = .2f;
		float orbit_speed = 180.0f;

		glm::vec2 last_mouse_position{};
		bool is_not_rotating = true;

		glm::vec3 target_point = { 0.0f, 0.0f, 0.0f };

	public:
		PerspectiveCameraController(float aspect_ratio);
		~PerspectiveCameraController() = default;

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

		void SetFieldOfView(float new_field_of_view);

	private:
		void UpdateOrbitPosition();

		bool OnWindowResized(Event& e);
		bool OnMouseScrolled(Event& e);
		bool OnToggleOrbitControl(Event& e);
	};
}