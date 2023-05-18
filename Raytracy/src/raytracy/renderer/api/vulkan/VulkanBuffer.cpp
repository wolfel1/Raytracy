#include "raytracypch.h"
#include "VulkanBuffer.h"

namespace raytracy {
	VulkanVertexBuffer::VulkanVertexBuffer(size_t size) {
	}
	VulkanVertexBuffer::VulkanVertexBuffer(float* vertices, size_t size) {
	}
	VulkanVertexBuffer::~VulkanVertexBuffer() {
	}
	void VulkanVertexBuffer::Bind() const {
	}
	void VulkanVertexBuffer::Unbind() const {
	}
	VulkanIndexBuffer::VulkanIndexBuffer(uint32_t* indices, uint32_t count) : IndexBuffer(count) {
	}
	VulkanIndexBuffer::~VulkanIndexBuffer() {
	}
	void VulkanIndexBuffer::Bind() const {
	}
	void VulkanIndexBuffer::Unbind() const {
	}
}