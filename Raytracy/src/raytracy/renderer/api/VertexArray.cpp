#include "raytracypch.h"
#include "VertexArray.h"

#include "RendererAPI.h"
#include "opengl/OpenGLVertexArray.h"

namespace raytracy {
	shared_ptr<VertexArray> VertexArray::Create() {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:
			RTY_ASSERT(false, "Renderer API None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return make_shared<OpenGLVertexArray>();
		default:
			break;
		}

		RTY_ASSERT(false, "Unknown RendereAPI!");
		return nullptr;
	}
}