#include "raytracypch.h"
#include "VulkanContext.h"

namespace raytracy {
	void VulkanContext::SetVSync(bool enabled) {
		if (enabled != window->IsVSync()) {
			RecreateSwapChain();
		}
	}
}