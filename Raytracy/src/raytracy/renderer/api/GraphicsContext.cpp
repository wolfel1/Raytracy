#include "raytracypch.h"
#include "GraphicsContext.h"

#include "RendererAPI.h"
#include "opengl/OpenGLContext.h"

namespace raytracy {
	shared_ptr<GraphicsContext> GraphicsContext::current_context;

	shared_ptr<GraphicsContext> GraphicsContext::Create() {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:
			RTY_ASSERT(false, "Renderer API None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			current_context = make_shared<OpenGLContext>();
			break;
		default:
			break;
		}

		RTY_ASSERT(current_context, "Unknown RendereAPI!");
		return current_context;
	}

}