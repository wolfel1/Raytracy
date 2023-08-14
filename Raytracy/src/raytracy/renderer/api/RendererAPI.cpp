#include "raytracypch.h"
#include "RendererAPI.h"

#include "vulkan/VulkanRendererAPI.h"
#include "opengl/OpenGLRendererAPI.h"

namespace raytracy {

	shared_ptr<RendererAPI> RendererAPI::current_api;

	shared_ptr<RendererAPI> RendererAPI::Create() {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:
			RTY_ASSERT(false, "Renderer API None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			current_api = make_shared<OpenGLRendererAPI>();
			break;
		case RendererAPI::API::Vulkan:
			current_api = make_shared<VulkanRendererAPI>();
			break;
		default:
			break;
		}

		RTY_ASSERT(current_api, "Unknown RendereAPI!");
		return current_api;
	}
}