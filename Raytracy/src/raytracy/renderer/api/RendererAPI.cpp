#include "raytracypch.h"
#include "RendererAPI.h"

#include "opengl/OpenGLRendererAPI.h"
#include "vulkan/VulkanRendererAPI.h"

namespace raytracy {
	unique_ptr<RendererAPI> RendererAPI::Create() {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:
			RTY_ASSERT(false, "Renderer API None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return make_unique<OpenGLRendererAPI>();
		case RendererAPI::API::Vulkan:
			return make_unique<VulkanRendererAPI>();
		default:
			break;
		}

		RTY_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}