#pragma once

#include "raytracypch.h"

namespace raytracy {

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory {
		None = 0,
		EventCategoryApplication,
		EventCategoryInput,
		EventCategoryKeyboard,
		EventCategoryMouse,
		EventCategoryMouseButton
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event {
	public:
		bool is_handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const {
			return GetName();
		}

		inline bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() == category;
		}
	};

	using EventCallbackFn = std::function<bool(Event&)>;

	class EventBus {

	private:
		std::unordered_map<EventType, std::vector<EventCallbackFn>> event_callback_map;

	public:
		EventBus(const EventBus&) = delete;

		template<typename T>
		void Register(const EventCallbackFn& callback) {
			RTY_BASE_ASSERT((std::is_base_of<Event, T>::value), "Provided type is not a subclass of Event!");

			auto node_handle = event_callback_map.extract((EventType) T::GetStaticType());
			if (node_handle.empty()) {
				std::vector<EventCallbackFn> callback_funcs;
				callback_funcs.push_back(callback);
				event_callback_map.insert({T::GetStaticType(), callback_funcs});
			} else {
				node_handle.mapped().push_back(callback);
			}
		}

		template<typename T>
		void Notify(T& evt) {
			RTY_BASE_ASSERT((std::is_base_of<Event, T>::value), "Provided type is not a subclass of Event!");
			if (!evt.is_handled) {
				if(auto pair = event_callback_map.find(evt.GetEventType()); pair != event_callback_map.end()) {
					for (const auto& callback : pair->second) {
						evt.is_handled = callback(evt);
					}
				} 
			}
		}

		static EventBus& Get() {
			static EventBus event_bus;
			return event_bus;
		}

	private:
		EventBus() {}
	};
}

