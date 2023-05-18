#pragma once

#include "VulkanBuffer.h"

namespace raytracy {
	class VulkanVertexArray {
	public:
		VulkanVertexArray();
		~VulkanVertexArray();

		 void Bind();
		 void Unbind();
		 void SetVertexBuffer(const shared_ptr<VulkanVertexBuffer>& vertex_buffer);
		 void SetIndexBuffer(const shared_ptr<VulkanIndexBuffer>& vertex_buffer);
	};
}