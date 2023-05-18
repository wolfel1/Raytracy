#pragma once

#include "../VertexArray.h"

namespace raytracy {
	class VulkanVertexArray : public VertexArray {
	public:
		VulkanVertexArray();
		~VulkanVertexArray();

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void SetVertexBuffer(const shared_ptr<VertexBuffer>& vertex_buffer) override;
		virtual void SetIndexBuffer(const shared_ptr<IndexBuffer>& vertex_buffer) override;
	};
}