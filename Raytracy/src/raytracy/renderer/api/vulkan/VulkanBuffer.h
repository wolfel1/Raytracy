#pragma once

#include "../Buffer.h"

#include <vulkan/vulkan.h>

namespace raytracy {

	class VulkanVertexBuffer : public VertexBuffer {

	private:
		VkDevice logical_device{};

		VkBuffer vertex_buffer{};
		VkDeviceMemory vertex_buffer_memory{};

	public:
		VulkanVertexBuffer(size_t size, shared_ptr<RendererAPI>& api);
		VulkanVertexBuffer(float* vertices, size_t size, shared_ptr<RendererAPI>& api);
		~VulkanVertexBuffer();
		
		const VkBuffer& GetBuffer() const { return vertex_buffer; }

		virtual void Bind() const override;
		virtual void Unbind() const override;
	};

	class VulkanIndexBuffer : public IndexBuffer {
	private:
		VkDevice logical_device{};

		VkBuffer index_buffer{};
		VkDeviceMemory index_buffer_memory{};
	public:
		VulkanIndexBuffer(uint32_t* indices, uint32_t count, shared_ptr<RendererAPI>& api);
		~VulkanIndexBuffer();

		const VkBuffer& GetBuffer() const { return index_buffer; }

		virtual void Bind() const override;
		virtual void Unbind() const override;
	};
}