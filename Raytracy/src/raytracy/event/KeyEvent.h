#pragma once

#include "Event.h"
#include "../core/KeyCodes.h"

namespace raytracy {

	class  KeyEvent : public Event {
	public:
		inline KeyCode GetKeyCode() const {
			return key_code;
		}

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(KeyCode keycode)
			: key_code(keycode) {
		}

		KeyCode key_code;
	};

	class  KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(KeyCode keycode, int repeatCount)
			: KeyEvent(keycode), repeat_count(repeatCount) {
		}

		inline int GetRepeatCount() const {
			return repeat_count;
		}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << key_code << " (" << repeat_count << " repeats)" << "\n";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int repeat_count;
	};

	class  KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(KeyCode keycode)
			: KeyEvent(keycode) {
		}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << key_code << "\n";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class  KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(KeyCode keycode)
			: KeyEvent(keycode) {
		}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent: " << key_code << "\n";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}