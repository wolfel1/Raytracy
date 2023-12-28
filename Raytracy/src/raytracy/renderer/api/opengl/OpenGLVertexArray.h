#pragma once

#include "OpenGLBuffer.h"

namespace raytracy {
	class OpenGLVertexArray {
	private:
		uint32_t renderer_id{};
		shared_ptr<OpenGLVertexBuffer> vertex_buffer;
		shared_ptr<OpenGLIndexBuffer> index_buffer;

	public:
		OpenGLVertexArray();
		~OpenGLVertexArray(); 
		
		static shared_ptr<OpenGLVertexArray> Create() {
			return make_shared<OpenGLVertexArray>();
		}

		void Bind();
		void Unbind();

		void SetVertexBuffer(const shared_ptr<OpenGLVertexBuffer> vertex_buffer);
		void SetIndexBuffer(const shared_ptr<OpenGLIndexBuffer> vertex_buffer);

		const shared_ptr<OpenGLVertexBuffer> GetVertexBuffer() const {
			return vertex_buffer;
		}

		const shared_ptr<OpenGLIndexBuffer> GetIndexBuffer() const {
			return index_buffer;
		}
	};
}