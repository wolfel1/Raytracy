#include "raytracypch.h"
#include "VulkanRendererAPI.h"

namespace raytracy {

	VulkanRendererAPI::VulkanRendererAPI() : clear_color({0.0f, 0.0f, 0.0f, 1.0f}) {
	}

	void VulkanRendererAPI::Init(const shared_ptr<GraphicsContext>& context) {
		graphics_context = std::dynamic_pointer_cast<VulkanContext>(context);
		RTY_ASSERT(graphics_context, "Context is not Vulkan!");

		CreateGraphicsPipeline();
		CreateCommandPool();
		CreateCommandBuffers();
		CreateSyncObjects();
	}


	void VulkanRendererAPI::ClearViewport() {
	}

	void VulkanRendererAPI::SetClearColor(const glm::vec4& clear_color) {
		this->clear_color = clear_color;
	}

	void VulkanRendererAPI::DrawIndexed(const shared_ptr<VertexArray>& vertex_array) {
		auto logical_device = graphics_context->GetLogicalDevice();
		auto swap_chain = graphics_context->GetSwapchain();

		vkWaitForFences(logical_device, 1, &in_flight_fences[current_frame], VK_TRUE, UINT64_MAX);

		uint32_t image_index;
		VkResult result = vkAcquireNextImageKHR(logical_device, swap_chain, UINT64_MAX,
			image_available_semaphores[current_frame], VK_NULL_HANDLE, &image_index);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			graphics_context->RecreateSwapChain();
			return;
		} else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("Failed to acquire swap chain images!");
		}
		vkResetFences(logical_device, 1, &in_flight_fences[current_frame]);

		vkResetCommandBuffer(command_buffers[current_frame], 0);
		RecordCommandBuffer(command_buffers[current_frame], image_index, vertex_array);

		VkSubmitInfo submit_info{};
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		VkSemaphore wait_semaphores[] = { image_available_semaphores[current_frame] };
		VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submit_info.waitSemaphoreCount = 1;
		submit_info.pWaitSemaphores = wait_semaphores;
		submit_info.pWaitDstStageMask = wait_stages;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &command_buffers[current_frame];

		VkSemaphore signal_semaphores[] = { render_finished_semaphores[current_frame] };
		submit_info.signalSemaphoreCount = 1;
		submit_info.pSignalSemaphores = signal_semaphores;

		auto graphics_queue = graphics_context->GetGraphicsQueue();
		if (vkQueueSubmit(graphics_queue, 1, &submit_info, in_flight_fences[current_frame]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to submit draw command buffer!");
		}

		VkPresentInfoKHR present_info{};
		present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present_info.waitSemaphoreCount = 1;
		present_info.pWaitSemaphores = signal_semaphores;

		VkSwapchainKHR swap_chains[] = { swap_chain };
		present_info.swapchainCount = 1;
		present_info.pSwapchains = swap_chains;
		present_info.pImageIndices = &image_index;
		present_info.pResults = nullptr;

		auto presentation_queue = graphics_context->GetPresentationQueue();
		result = vkQueuePresentKHR(presentation_queue, &present_info);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebuffer_resized) {
			framebuffer_resized = false;
			graphics_context->RecreateSwapChain();
		} else if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to present swap chain image!");
		}

		current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void VulkanRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		framebuffer_resized = true;
	}

	void VulkanRendererAPI::Shutdown() {
		auto logical_device = graphics_context->GetLogicalDevice();
		vkDeviceWaitIdle(logical_device);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(logical_device, image_available_semaphores[i], nullptr);
			vkDestroySemaphore(logical_device, render_finished_semaphores[i], nullptr);
			vkDestroyFence(logical_device, in_flight_fences[i], nullptr);
		}
		vkDestroyCommandPool(logical_device, command_pool, nullptr);
		vkDestroyPipeline(logical_device, graphics_pipeline, nullptr);
		vkDestroyPipelineLayout(logical_device, pipeline_layout, nullptr);

		graphics_context->Shutdown();
	}
}