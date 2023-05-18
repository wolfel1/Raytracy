#include "raytracypch.h"
#include "VulkanRendererAPI.h"

namespace raytracy {
	void VulkanRendererAPI::Init(shared_ptr<VulkanContext> context) {
		graphics_context = context;
	}
	void VulkanRendererAPI::ClearViewport() {
	}
	void VulkanRendererAPI::SetClearColor(const glm::vec4& clear_color) {
	}
	void VulkanRendererAPI::DrawIndexed(const shared_ptr<VulkanIndexBuffer>& index_buffer) {
	}
	void VulkanRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
	}
	void VulkanRendererAPI::Shutdown() {
	}
}