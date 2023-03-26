#pragma once

namespace raytracy {
#ifdef RTY_DEBUG
#define GLCall(x) GLClearError();x;GLLogCall(#x, __FILE__, __LINE__)
	void GLClearError();

	bool GLLogCall(const char* function, const char* file, int line);
#else
#define GLCall(x) x
#endif
}