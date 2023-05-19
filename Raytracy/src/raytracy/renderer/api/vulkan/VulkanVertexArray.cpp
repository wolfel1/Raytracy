#include "raytracypch.h"
#include "VulkanVertexArray.h"

namespace raytracy {
	VulkanVertexArray::VulkanVertexArray() {
	}
	VulkanVertexArray::~VulkanVertexArray() {
	}
	void VulkanVertexArray::Bind() {
	}
	void VulkanVertexArray::Unbind() {
	}
	void VulkanVertexArray::SetVertexBuffer(const shared_ptr<VertexBuffer>& vertex_buffer) {
		this->vertex_buffer = vertex_buffer;
	}
	void VulkanVertexArray::SetIndexBuffer(const shared_ptr<IndexBuffer>& index_buffer) {
		this->index_buffer = index_buffer;
	}
}