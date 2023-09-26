#pragma once

#include "../Buffer.h"

#include <vulkan/vulkan.h>
#include "VulkanContext.h"

namespace raytracy {

	class VulkanVertexBuffer : public VertexBuffer {

	private:
		VkDevice logical_device{};

		VkBuffer vertex_buffer{};
		VkDeviceMemory vertex_buffer_memory{};

	public:
		VulkanVertexBuffer(size_t size);
		VulkanVertexBuffer(std::vector<Vertex>& vertices);
		~VulkanVertexBuffer();
		
		const VkBuffer& GetBuffer() const { return vertex_buffer; }
	};

	class VulkanIndexBuffer : public IndexBuffer {
	private:
		VkDevice logical_device{};

		VkBuffer index_buffer{};
		VkDeviceMemory index_buffer_memory{};
	public:
		VulkanIndexBuffer(uint32_t* indices, uint32_t count);
		~VulkanIndexBuffer();

		const VkBuffer& GetBuffer() const { return index_buffer; }
	};

	class VulkanUniformBuffer : public UniformBuffer {

	private:
		std::vector<VkBuffer> uniform_buffers;
		std::vector<VkDeviceMemory> uniform_buffers_memory;
		std::vector<void*> uniform_buffers_mapped;
		VkDescriptorPool descriptor_pool;
		std::vector<VkDescriptorSet> descriptor_sets;
		VkDescriptorSetLayout descriptor_set_layout{};

	public:
		VulkanUniformBuffer(const BufferLayout& layout);
		~VulkanUniformBuffer();

		const std::vector<VkDescriptorSet>& GetDescriptorSets() const { return descriptor_sets; }

		virtual void Link(uint32_t const index) const override;

		virtual void SetVec4(const std::string& name, const glm::vec4& value) const override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) const override;

	private:
		void CreateUniformBuffers(const BufferLayout& layout, const shared_ptr<VulkanContext> context);
		void CreateDescriptorPool(const shared_ptr<VulkanContext> context);
		void CreateDescriptorSets(const BufferLayout& layout, const shared_ptr<VulkanContext> context);
	};
}