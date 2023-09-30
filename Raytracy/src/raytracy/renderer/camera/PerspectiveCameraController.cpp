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

	bool PerspectiveCameraController::OnWindowResized(Event& e) {
		return true;
	}

	bool PerspectiveCameraController::OnMouseScrolled(Event& e) {
		return true;
	}
}