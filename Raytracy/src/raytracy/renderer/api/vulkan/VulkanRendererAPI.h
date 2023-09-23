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
	public:
		static const int MAX_FRAMES_IN_FLIGHT = 2;
		static uint32_t CURRENT_FRAME;
	private:

		bool framebuffer_resized = false;
		glm::vec4 clear_color;

		shared_ptr<VulkanContext> graphics_context;

		VkDescriptorSetLayout descriptor_set_layout{};
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

		const VkCommandPool& GetCommandPool() const { return command_pool; }
		const VkDescriptorSetLayout& GetDescriptorSetLayout() const { return descriptor_set_layout; }

		virtual void ClearViewport() override;

		virtual void SetClearColor(const glm::vec4& clear_color) override;

		virtual void DrawIndexed(const shared_ptr<VertexArray> vertex_array, std::unordered_map<std::string, shared_ptr<UniformBuffer>> const& uniform_buffer) override;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void Shutdown() override;

		private:
			void CreateDescriptorSetLayout();
			bool CreateGraphicsPipeline(Event& e);
			void CreateCommandPool();
			void CreateCommandBuffers();
			void CreateSyncObjects();
			void RecordCommandBuffer(VkCommandBuffer command_buffer, uint32_t image_index, const shared_ptr<VertexArray> vertex_array, std::unordered_map<std::string, shared_ptr<UniformBuffer>> const& uniform_buffer);
	};
}