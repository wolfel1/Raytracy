#pragma once

#include "Event.h"
#include "../core/Timestep.h"
#include "raytracy/renderer/api/Shader.h"

namespace raytracy {
	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() = default;
		~WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class WindowResizeEvent : public Event {
	private:
		unsigned int width, height;

	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: width(width), height(height) {
		}
		~WindowResizeEvent() = default;

		inline unsigned int GetWidth() const {
			return width;
		}
		inline unsigned int GetHeight() const {
			return height;
		}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << width << ", " << height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};


	class AppTickEvent : public Event {
	private:
		Timestep timestep;
	public:
		AppTickEvent(Timestep t) : timestep(t) {};
		~AppTickEvent() = default;
		
		Timestep GetTimeStep() const {
			return timestep;
		}
		
		std::string ToString() const override {
			std::stringstream ss;
			ss << "AppTickEvent: time" << timestep.GetMilliseconds();
			return ss.str();
		}

		EVENT_CLASS_TYPE(AppTick);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};
}