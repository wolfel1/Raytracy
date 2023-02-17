#pragma once

#include "Event.h"

namespace raytracy {

	class  KeyEvent : public Event {
	public:
		inline uint16_t GetKeyCode() const {
			return m_KeyCode;
		}

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(uint16_t keycode)
			: m_KeyCode(keycode) {
		}

		uint16_t m_KeyCode;
	};

	class  KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(uint16_t keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {
		}

		inline int GetRepeatCount() const {
			return m_RepeatCount;
		}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)" << "\n";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_RepeatCount;
	};

	class  KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(uint16_t keycode)
			: KeyEvent(keycode) {
		}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode << "\n";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class  KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(uint16_t keycode)
			: KeyEvent(keycode) {
		}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode << "\n";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}