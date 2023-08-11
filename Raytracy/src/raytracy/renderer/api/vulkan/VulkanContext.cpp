#include "raytracypch.h"
#include "VulkanContext.h"

namespace raytracy {
	void VulkanContext::SetVSync(bool enabled) {
		if (enabled != m_window->IsVSync()) {
			RecreateSwapChain();
		}
	}
}