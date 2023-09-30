#include "raytracypch.h"
#include "PerspectiveCameraController.h"

namespace raytracy {

	PerspectiveCameraController::PerspectiveCameraController(float aspect_ratio) : aspect_ratio(aspect_ratio), camera(field_of_view, aspect_ratio) {
		auto& event_bus = EventBus::Get();
		event_bus.Register<WindowResizeEvent>(RTY_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
		event_bus.Register<MouseScrolledEvent>(RTY_BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled));
	}

	void PerspectiveCameraController::OnUpdate(Timestep timestep) {
	}

	void PerspectiveCameraController::OnEvent(Event& e) {
	}

	void PerspectiveCameraController::Translate(glm::vec3 const& amount) {
		auto& camera_position = camera.GetPosition();
		glm::vec3 new_position = camera_position + amount;
		camera.SetPosition(new_position);
	}

	bool PerspectiveCameraController::OnWindowResized(Event& e) {
		return true;
	}

	bool PerspectiveCameraController::OnMouseScrolled(Event& e) {
		auto& evt = static_cast<WindowResizeEvent&>(e);
		aspect_ratio = (float)evt.GetWidth() / (float)evt.GetHeight();
		camera.SetProjection(field_of_view, aspect_ratio);
		return true;
	}
}