#include "raytracypch.h"
#include "GraphicsContext.h"

#include "RendererAPI.h"
#include "opengl/OpenGLContext.h"
#include "vulkan/VulkanContext.h"

namespace raytracy {
	shared_ptr<GraphicsContext> GraphicsContext::Create() {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:
			RTY_ASSERT(false, "Renderer API None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return make_shared<OpenGLContext>();
		case RendererAPI::API::Vulkan:
			return make_shared<VulkanContext>();
		default:
			break;
		}

		RTY_ASSERT(false, "Unknown RendereAPI!");
		return nullptr;
	}

}