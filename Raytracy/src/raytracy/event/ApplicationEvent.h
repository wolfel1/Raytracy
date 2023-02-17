#pragma once

#include "Event.h"

namespace raytracy {
	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() = default;
		~WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};
}