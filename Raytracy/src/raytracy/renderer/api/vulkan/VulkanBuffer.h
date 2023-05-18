#pragma once
#include "../Buffer.h"

namespace raytracy {
	class VulkanVertexBuffer : public VertexBuffer {
	public:
		VulkanVertexBuffer(size_t size);
		VulkanVertexBuffer(float* vertices, size_t size);
		~VulkanVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
	};

	class VulkanIndexBuffer : public IndexBuffer {

	public:
		VulkanIndexBuffer(uint32_t* indices, uint32_t count);
		~VulkanIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
	};
}