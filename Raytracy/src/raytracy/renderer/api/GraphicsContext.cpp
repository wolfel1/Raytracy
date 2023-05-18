#include "raytracypch.h"
#include "GraphicsContext.h"

#include "RendererAPI.h"
#include "opengl/OpenGLContext.h"
#include "vulkan/VulkanContext.h"

namespace raytracy {
	shared_ptr<GraphicsContext> GraphicsContext::Create(void* window_handle) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:
			RTY_ASSERT(false, "Renderer API None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return make_shared<OpenGLContext>(window_handle);
		case RendererAPI::API::Vulkan:
			return make_shared<VulkanContext>(window_handle);
		default:
			break;
		}

		RTY_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	GraphicsContext::GraphicsContext(void* window_handle) : window_handle(window_handle) {
		RTY_ASSERT(window_handle, "Window handle is null!");
	}
}