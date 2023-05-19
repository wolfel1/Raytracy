#include "raytracypch.h"
#include "RendererAPI.h"

#include "vulkan/VulkanRendererAPI.h"
#include "opengl/OpenGLRendererAPI.h"

namespace raytracy {
	shared_ptr<RendererAPI> RendererAPI::Create() {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:
			RTY_ASSERT(false, "Renderer API None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return make_shared<OpenGLRendererAPI>();
		case RendererAPI::API::Vulkan:
			return make_shared<VulkanRendererAPI>();
		default:
			break;
		}

		RTY_ASSERT(false, "Unknown RendereAPI!");
		return nullptr;
	}
}