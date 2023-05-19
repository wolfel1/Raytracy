#include "raytracypch.h"
#include "RendererAPI.h"

#include "vulkan/VulkanRendererAPI.h"

namespace raytracy {
	shared_ptr<RendererAPI> RendererAPI::Create() {
		return make_shared<VulkanRendererAPI>();
	}
}