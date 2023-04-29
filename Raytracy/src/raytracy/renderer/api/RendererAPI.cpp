#include "raytracypch.h"
#include "RendererAPI.h"

#include "opengl/OpenGLRendererAPI.h"

namespace raytracy {
	unique_ptr<RendererAPI> RendererAPI::Create() {
		return make_unique<OpenGLRendererAPI>();
	}
}