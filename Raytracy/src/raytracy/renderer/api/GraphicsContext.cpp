#include "raytracypch.h"
#include "GraphicsContext.h"

#include "opengl/OpenGLContext.h"

namespace raytracy {
	unique_ptr<GraphicsContext> GraphicsContext::Create(void* window_handle) {
		return make_unique<OpenGLContext>(window_handle);
	}
}