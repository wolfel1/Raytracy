#include "raytracypch.h"
#include "VulkanContext.h"

namespace raytracy {
	VulkanContext::VulkanContext(const shared_ptr<Window>& window) : GraphicsContext(window) {
	}
	void VulkanContext::SetVSync(bool enabled) {
		if (enabled != window->IsVSync()) {
			RecreateSwapChain();

		}
	}
}