#pragma once

#include "PerspectiveCamera.h"
#include "raytracy/core/Timestep.h"

#include "raytracy/event/ApplicationEvent.h"
#include "raytracy/event/MouseEvent.h"

namespace raytracy {

	class PerspectiveCameraController {

	private:
		float aspect_ratio;
		float field_of_view = 90.0f;
		PerspectiveCamera camera;

		float translation_speed = 5.0f;
		float rollation_speed = 180.0f;
		float orbit_speed = 180.0f;

	public:
		PerspectiveCameraController(float aspect_ratio);

		void OnUpdate(Timestep timestep);
		void OnEvent(Event& e);

		PerspectiveCamera const& GetCamera() const {
			return camera;
		}

	private:
		bool OnWindowResized(Event& e);
		bool OnMouseScrolled(Event& e);
	};
}