#pragma once

#include <cstdint>
#include <ostream>

namespace raytracy {
	typedef enum class MouseCode : uint16_t {
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode) {
		os << static_cast<int32_t>(mouseCode);
		return os;
	}

	typedef enum class CursorMode : uint16_t {
		// From glfw3.h
		NormalCursor = 0x00034001,
		HiddenCursor = 0x00034002,
		DisabledCursor = 0x00034003
	};
}
