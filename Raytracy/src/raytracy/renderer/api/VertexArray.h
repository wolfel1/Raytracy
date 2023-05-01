#pragma once

#include "Buffer.h"

namespace raytracy {
	class VertexArray {
		protected:
		uint32_t renderer_id{};

		public:
		virtual ~VertexArray() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetVertexBuffer(const shared_ptr<VertexBuffer>& vertex_buffer) = 0;
		virtual void SetIndexBuffer(const shared_ptr<IndexBuffer>& index_buffer) = 0;

		static shared_ptr<VertexArray> Create();
	};
}
