#include "raytracypch.h"
#include "RendererAPI.h"

#include <glad/gl.h>

namespace raytracy {
	void GLClearError() {
		while (glGetError() != GL_NO_ERROR);
	}

	bool GLLogCall(const char* function, const char* file, int line) {
		while (GLenum error = glGetError()) {
			RTY_RENDERER_ERROR("[OpenGL Error] ({0}) in {1}, {2} at {3}", error, function, file, line);
			return false;
		}
		return true;
	}
}