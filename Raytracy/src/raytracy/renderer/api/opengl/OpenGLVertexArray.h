#pragma once

#include "../VertexArray.h"

namespace raytracy {
	class OpenGLVertexArray : public VertexArray {
		private:
		shared_ptr<VertexBuffer> vertex_buffer;
		shared_ptr<IndexBuffer> index_buffer;
		public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void SetVertexBuffer(const shared_ptr<VertexBuffer>& vertex_buffer) override;
		virtual void SetIndexBuffer(const shared_ptr<IndexBuffer>& vertex_buffer) override;

		const shared_ptr<VertexBuffer>& GetVertexBuffer() const {
			return vertex_buffer;
		}

		const shared_ptr<IndexBuffer>& GetIndexBuffer() const {
			return index_buffer;
		}
	};
}