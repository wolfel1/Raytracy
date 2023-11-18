#pragma once

#include "Buffer.h"

namespace raytracy {
	class VertexArray {
	protected:
		uint32_t renderer_id{};
		shared_ptr<VertexBuffer> vertex_buffer;
		shared_ptr<IndexBuffer> index_buffer;

	public:
		virtual ~VertexArray() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetVertexBuffer(const shared_ptr<VertexBuffer>& vertex_buffer) = 0;
		virtual void SetIndexBuffer(const shared_ptr<IndexBuffer>& index_buffer) = 0;

		const shared_ptr<VertexBuffer> GetVertexBuffer() const {
			return vertex_buffer;
		}

		const shared_ptr<IndexBuffer> GetIndexBuffer() const {
			return index_buffer;
		}

		static shared_ptr<VertexArray> Create();
	};
}
