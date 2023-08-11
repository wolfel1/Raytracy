#include "raytracypch.h"
#include "VulkanBuffer.h"

#include "VulkanRendererAPI.h"
#include "../../Renderer.h"


namespace raytracy {
	static uint32_t FindMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties, VkPhysicalDevice& physical_device) {
		VkPhysicalDeviceMemoryProperties memory_properties{};
		vkGetPhysicalDeviceMemoryProperties(physical_device, &memory_properties);

		for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++) {
			if (type_filter & (1 << i) && (memory_properties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}
		throw std::runtime_error("Failed to find suitable memory type!");
	}

	static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer,
		VkDeviceMemory& buffer_memory, const shared_ptr<VulkanContext>& context) {

		auto logical_device = context->GetLogicalDevice();
		auto physical_device = context->GetPhysicalDevice();

		VkBufferCreateInfo buffer_info{};
		buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		buffer_info.size = size;
		buffer_info.usage = usage;
		buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(logical_device, &buffer_info, nullptr, &buffer) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create vertex buffer!");
		}

		VkMemoryRequirements memory_requirements{};
		vkGetBufferMemoryRequirements(logical_device, buffer, &memory_requirements);

		VkMemoryAllocateInfo alloc_info{};
		alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		alloc_info.allocationSize = memory_requirements.size;
		alloc_info.memoryTypeIndex = FindMemoryType(memory_requirements.memoryTypeBits, properties, physical_device);

		//Not ideal for large objects/scenes
		if (vkAllocateMemory(logical_device, &alloc_info, nullptr, &buffer_memory) != VK_SUCCESS) {
			throw std::runtime_error("Failed to allocate vertex buffer memory!");
		}

		vkBindBufferMemory(logical_device, buffer, buffer_memory, 0);
	}

	static void CopyBuffer(VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size, const shared_ptr<VulkanRendererAPI>& api) {
		auto command_pool = api->GetCommandPool();
		auto vulkan_context = api->GetContext();
		auto logical_device = vulkan_context->GetLogicalDevice();
		auto graphics_queue = vulkan_context->GetGraphicsQueue();

		VkCommandBufferAllocateInfo alloc_info{};
		alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		alloc_info.commandPool = command_pool;
		alloc_info.commandBufferCount = 1;

		VkCommandBuffer command_buffer{};
		vkAllocateCommandBuffers(logical_device, &alloc_info, &command_buffer);

		VkCommandBufferBeginInfo begin_info{};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(command_buffer, &begin_info);

		VkBufferCopy copy_region{};
		copy_region.srcOffset = 0;
		copy_region.dstOffset = 0;
		copy_region.size = size;
		vkCmdCopyBuffer(command_buffer, src_buffer, dst_buffer, 1, &copy_region);

		vkEndCommandBuffer(command_buffer);

		VkSubmitInfo submit_info{};
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &command_buffer;

		vkQueueSubmit(graphics_queue, 1, &submit_info, VK_NULL_HANDLE);
		vkQueueWaitIdle(graphics_queue);

		vkFreeCommandBuffers(logical_device, command_pool, 1, &command_buffer);
	}

	VulkanVertexBuffer::VulkanVertexBuffer(size_t size, const shared_ptr<RendererAPI>& api) {
	}

	VulkanVertexBuffer::VulkanVertexBuffer(std::vector<Vertex>& vertices, const shared_ptr<RendererAPI>& api) {
		auto vulkan_api = std::dynamic_pointer_cast<VulkanRendererAPI>(api);
		RTY_ASSERT(vulkan_api, "Api is not Vulkan!");
		auto vulkan_context = vulkan_api->GetContext();

		for (auto& vertex : vertices) {
			vertex.position.y *= -1;
		}
		VkDeviceSize buffer_size = sizeof(Vertex) * vertices.size();
		logical_device = vulkan_context->GetLogicalDevice();

		VkBuffer staging_buffer{};
		VkDeviceMemory staging_buffer_memory{};
		CreateBuffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			staging_buffer, staging_buffer_memory, vulkan_context);

		void* data;
		vkMapMemory(logical_device, staging_buffer_memory, 0, buffer_size, 0, &data);
		memcpy(data, vertices.data(), (size_t)buffer_size);
		vkUnmapMemory(logical_device, staging_buffer_memory);

		CreateBuffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			vertex_buffer, vertex_buffer_memory, vulkan_context);

		CopyBuffer(staging_buffer, vertex_buffer, buffer_size, vulkan_api);

		vkDestroyBuffer(logical_device, staging_buffer, nullptr);
		vkFreeMemory(logical_device, staging_buffer_memory, nullptr);
	}

	VulkanVertexBuffer::~VulkanVertexBuffer() {
		vkDestroyBuffer(logical_device, vertex_buffer, nullptr);
		vkFreeMemory(logical_device, vertex_buffer_memory, nullptr);
	}
	VulkanIndexBuffer::VulkanIndexBuffer(uint32_t* indices, uint32_t count, shared_ptr<RendererAPI>& api) : IndexBuffer(count) {
		auto vulkan_api = std::dynamic_pointer_cast<VulkanRendererAPI>(api);
		RTY_ASSERT(api, "Api is not Vulkan!");
		auto vulkan_context = vulkan_api->GetContext();

		VkDeviceSize buffer_size = sizeof(uint32_t) * count;
		logical_device = vulkan_context->GetLogicalDevice();

		VkBuffer staging_buffer{};
		VkDeviceMemory staging_buffer_memory{};
		CreateBuffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			staging_buffer, staging_buffer_memory, vulkan_context);

		void* data;
		vkMapMemory(logical_device, staging_buffer_memory, 0, buffer_size, 0, &data);
		memcpy(data, indices, (size_t)buffer_size);
		vkUnmapMemory(logical_device, staging_buffer_memory);

		CreateBuffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			index_buffer, index_buffer_memory, vulkan_context);

		CopyBuffer(staging_buffer, index_buffer, buffer_size, vulkan_api);

		vkDestroyBuffer(logical_device, staging_buffer, nullptr);
		vkFreeMemory(logical_device, staging_buffer_memory, nullptr);
	}
	VulkanIndexBuffer::~VulkanIndexBuffer() {
		vkDestroyBuffer(logical_device, index_buffer, nullptr);
		vkFreeMemory(logical_device, index_buffer_memory, nullptr);
	}

	
}