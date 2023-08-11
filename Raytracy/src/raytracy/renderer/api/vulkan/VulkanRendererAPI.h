#pragma once

#include "../RendererAPI.h"
#include "VulkanContext.h"
#include "VulkanVertexArray.h"
#include "VulkanBuffer.h"
#include "VulkanShader.h"

#include <vulkan/vulkan.h>
#include "raytracy/event/ApplicationEvent.h"


namespace raytracy {

	class VulkanRendererAPI : public RendererAPI {

	private:
		const int MAX_FRAMES_IN_FLIGHT = 2;

		bool framebuffer_resized = false;
		uint32_t current_frame = 0;
		glm::vec4 clear_color;

		shared_ptr<VulkanContext> graphics_context;

		VkPipelineLayout pipeline_layout{};
		VkPipeline graphics_pipeline{};
		VkCommandPool command_pool{};
		std::vector<VkCommandBuffer> command_buffers;

		std::vector<VkSemaphore> image_available_semaphores;
		std::vector<VkSemaphore> render_finished_semaphores;
		std::vector<VkFence> in_flight_fences;
	public:
		VulkanRendererAPI();

		virtual void Init() override;

		const shared_ptr<VulkanContext> GetContext() const { return graphics_context; }

		const VkCommandPool GetCommandPool() const { return command_pool; }

		virtual void ClearViewport() override;

		virtual void SetClearColor(const glm::vec4& clear_color) override;

		virtual void DrawIndexed(const shared_ptr<VertexArray>& vertex_array) override;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void Shutdown() override;

		private:
			bool CreateGraphicsPipeline(Event& e);
			void CreateCommandPool();
			void CreateCommandBuffers();
			void CreateSyncObjects();
			void RecordCommandBuffer(VkCommandBuffer command_buffer, uint32_t image_index, const shared_ptr<VertexArray>& vertex_array);
	};
}