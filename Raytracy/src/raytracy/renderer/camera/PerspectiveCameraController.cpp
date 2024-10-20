#include "raytracypch.h"
#include "PerspectiveCameraController.h"

#include "raytracy/Application.h"
#include "raytracy/core/Input.h"
#include "glm/gtx/rotate_vector.hpp"

#include "raytracy/event/KeyEvent.h"
#include "raytracy/event/ApplicationEvent.h"
#include "raytracy/event/MouseEvent.h"

namespace raytracy {

	PerspectiveCameraController::PerspectiveCameraController(float aspect_ratio) : aspect_ratio(aspect_ratio) {
		camera = make_shared<PerspectiveCamera>(field_of_view, aspect_ratio);

		auto& event_bus = EventBus::Get();
		event_bus.Register<WindowResizeEvent>(RTY_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
		event_bus.Register<MouseScrolledEvent>(RTY_BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled)); 
		event_bus.Register<KeyReleasedEvent>(RTY_BIND_EVENT_FN(PerspectiveCameraController::OnToggleOrbitControl));
	}

	void PerspectiveCameraController::OnUpdate(Timestep timestep) {
		auto& input = Input::Get();

		if (input.IsMouseButtonPressed<>(MouseCode::ButtonMiddle)) {
				
			if (is_not_rotating) {
				last_mouse_position = input.GetMousePosition<>();
				is_not_rotating = false;
			}
			auto mouse_position = input.GetMousePosition<>();
			float x_offset = (mouse_position.x - last_mouse_position.x);
			float y_offset = (last_mouse_position.y - mouse_position.y);
			if (input.IsKeyPressed<>(KeyCode::LeftShift)) {
				glm::vec3 translate_direction{};
				auto& up = camera->GetUp();
				auto& direction = camera->GetDirection();

				if (y_offset < 0) {
					translate_direction += glm::normalize(up);
				} else if (y_offset > 0) {
					translate_direction -= glm::normalize(up);
				}
				if (x_offset < 0) {
					translate_direction += glm::normalize(glm::cross(direction, up));
				} else if (x_offset > 0) {
					translate_direction -= glm::normalize(glm::cross(direction, up));
				}
				Translate(translate_direction * translation_speed);
			} else {
				camera->SetYaw(x_offset * rotation_speed);
				camera->SetPitch(y_offset * rotation_speed); 
				
				if(has_orbit_control) {
					UpdateOrbitPosition();
				}
			} 
			last_mouse_position = mouse_position;
		} else {
			is_not_rotating = true;
		}
			
	}

	void PerspectiveCameraController::Translate(glm::vec3 const& amount) {
		auto& camera_position = camera->GetPosition();
		glm::vec3 new_position = camera_position + amount;
		camera->SetPosition(new_position);
	}

	void PerspectiveCameraController::TranslateX(float const amount) {
		auto& camera_position = camera->GetPosition();
		glm::vec3 new_position = camera_position + glm::vec3(amount, 0.0f, 0.0f);
		camera->SetPosition(new_position);
	}

	void PerspectiveCameraController::TranslateY(float const amount) {
		auto& camera_position = camera->GetPosition();
		glm::vec3 new_position = camera_position + glm::vec3(0.0f, amount, 0.0f);
		camera->SetPosition(new_position); 
	}
	void PerspectiveCameraController::TranslateZ(float const amount) {
		auto& camera_position = camera->GetPosition();
		glm::vec3 new_position = camera_position + glm::vec3(0.0f, 0.0f, amount);
		camera->SetPosition(new_position); 
	}

	void PerspectiveCameraController::RotateX(float const degree) {
		camera->SetPitch(degree);

		if(has_orbit_control) {
			UpdateOrbitPosition();
		}
	}

	void PerspectiveCameraController::RotateY(float const degree) {
		camera->SetYaw(degree);

		if(has_orbit_control) {
			UpdateOrbitPosition();
		}
	}

	void PerspectiveCameraController::UpdateOrbitPosition() {
		float radius = glm::length(camera->GetPosition() - target_point);
		float yaw_radians = glm::radians(camera->GetYaw());
		float pitch_radians = glm::radians(camera->GetPitch());

		glm::vec3 new_position;
		new_position.x = target_point.x + radius * cos(pitch_radians) * cos(yaw_radians);
		new_position.y = target_point.y + radius * sin(pitch_radians);
		new_position.z = target_point.z + radius * cos(pitch_radians) * sin(yaw_radians);

		camera->SetPosition(new_position);
		camera->SetDirection(glm::normalize(target_point - new_position));
	}

	bool PerspectiveCameraController::OnWindowResized(Event& e) {
		auto& evt = static_cast<WindowResizeEvent&>(e);
		aspect_ratio = (float)evt.GetWidth() / (float)evt.GetHeight();
		camera->SetProjection(field_of_view, aspect_ratio);
		return true;
	}

	bool PerspectiveCameraController::OnMouseScrolled(Event& e) {
		auto& evt = static_cast<MouseScrolledEvent&>(e);
		auto y = evt.GetYOffset();
		glm::vec3 translate_direction{};
		auto& direction = camera->GetDirection();
		
		if (y < 0) {
			translate_direction -= glm::normalize(direction);
		} else if (y > 0) {
			translate_direction += glm::normalize(direction);
		}
		Translate(translate_direction * scroll_speed);
		
		return true;
	}

	bool PerspectiveCameraController::OnToggleOrbitControl(Event& e) {
		KeyReleasedEvent evt = static_cast<KeyReleasedEvent&>(e);
		if (evt.GetKeyCode() == KeyCode::O) {
			has_orbit_control = !has_orbit_control;
		}

		return true;
	}
}