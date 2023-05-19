#include "raytracypch.h"
#include "Buffer.h"

#include "vulkan/VulkanBuffer.h"


namespace raytracy {
	shared_ptr<VertexBuffer> VertexBuffer::Create(size_t size, shared_ptr<RendererAPI>& api) {
		return make_shared<VulkanVertexBuffer>(size, api);
	}

	shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, size_t size, shared_ptr<RendererAPI>& api) {
		return make_shared<VulkanVertexBuffer>(vertices, size, api);
	}

	shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count, shared_ptr<RendererAPI>& api) {
		return make_shared<VulkanIndexBuffer>(indices, count, api);
	}
}