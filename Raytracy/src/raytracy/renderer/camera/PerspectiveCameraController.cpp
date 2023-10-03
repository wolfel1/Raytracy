#include "raytracypch.h"
#include "PerspectiveCameraController.h"

#include "raytracy/Application.h"
#include "raytracy/core/Input.h"
#include "glm/gtx/rotate_vector.hpp"

namespace raytracy {

	PerspectiveCameraController::PerspectiveCameraController(float aspect_ratio) : aspect_ratio(aspect_ratio), camera(field_of_view, aspect_ratio) {

		auto& event_bus = EventBus::Get();
		event_bus.Register<WindowResizeEvent>(RTY_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
		event_bus.Register<MouseScrolledEvent>(RTY_BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled));
	}

	void PerspectiveCameraController::OnUpdate(Timestep timestep) {
		auto& input = Input::Get();

		if (input.IsMouseButtonPressed(MouseCode::Button1)) {
			if (is_not_rotating) {
				last_mouse_position = input.GetMousePosition();
				is_not_rotating = false;
			}
			auto mouse_position = input.GetMousePosition();
			float x_offset = (mouse_position.x - last_mouse_position.x);
			float y_offset = (last_mouse_position.y - mouse_position.y);
			if (input.IsKeyPressed(KeyCode::LeftShift)) {
				glm::vec3 translate_direction{};
				auto& up = camera.GetUp();
				auto& direction = camera.GetDirection();

				if (y_offset < 0) {
					translate_direction -= glm::normalize(up);
				} else if (y_offset > 0) {
					translate_direction += glm::normalize(up);
				}
				if (x_offset < 0) {
					translate_direction -= glm::normalize(glm::cross(direction, up));
				} else if (x_offset > 0) {
					translate_direction += glm::normalize(glm::cross(direction, up));
				}
				Translate(translate_direction * translation_speed);
			} else {
				camera.SetYaw(x_offset * rotation_speed);
				camera.SetPitch(y_offset * rotation_speed);
			} 
			last_mouse_position = mouse_position;
		} else {
			is_not_rotating = true;
		}
			
	}

	void PerspectiveCameraController::Translate(glm::vec3 const& amount) {
		auto& camera_position = camera.GetPosition();
		glm::vec3 new_position = camera_position + amount;
		camera.SetPosition(new_position);
	}

	void PerspectiveCameraController::TranslateX(float const amount) {
		auto& camera_position = camera.GetPosition();
		glm::vec3 new_position = camera_position + glm::vec3(amount, 0.0f, 0.0f);
		camera.SetPosition(new_position);
	}

	void PerspectiveCameraController::TranslateY(float const amount) {
		auto& camera_position = camera.GetPosition();
		glm::vec3 new_position = camera_position + glm::vec3(0.0f, amount, 0.0f);
		camera.SetPosition(new_position);
	}
	void PerspectiveCameraController::TranslateZ(float const amount) {
		auto& camera_position = camera.GetPosition();
		glm::vec3 new_position = camera_position + glm::vec3(0.0f, 0.0f, amount);
		camera.SetPosition(new_position);
	}

	void PerspectiveCameraController::RotateX(float const degree) {
		camera.SetPitch(degree);
	}

	void PerspectiveCameraController::RotateY(float const degree) {
		camera.SetYaw(degree);
	}


	bool PerspectiveCameraController::OnWindowResized(Event& e) {
		auto& evt = static_cast<WindowResizeEvent&>(e);
		aspect_ratio = (float)evt.GetWidth() / (float)evt.GetHeight();
		camera.SetProjection(field_of_view, aspect_ratio);
		return true;
	}

	bool PerspectiveCameraController::OnMouseScrolled(Event& e) {
		return true;
	}
}