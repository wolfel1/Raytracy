#include "raytracypch.h"
#include "Buffer.h"

#include <glad/gl.h>

#include "RendererAPI.h"

namespace raytracy {
	VertexBuffer::VertexBuffer(size_t size) {
		GLCall(glCreateBuffers(1, &renderer_id));
		GLCall(glNamedBufferData(renderer_id, size, nullptr, GL_DYNAMIC_DRAW));
	}

	VertexBuffer::VertexBuffer(float* vertices, size_t size) {
		GLCall(glCreateBuffers(1, &renderer_id));
		GLCall(glNamedBufferData(renderer_id, size * sizeof(float), vertices, GL_STATIC_DRAW));
    }
	VertexBuffer::~VertexBuffer() {
		GLCall(glDeleteBuffers(1, &renderer_id));
	}
	 void VertexBuffer::Bind() const {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, renderer_id));
	}
	 void VertexBuffer::Unbind() const {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) : count(count) {
		GLCall(glCreateBuffers(1, &renderer_id));
		GLCall(glNamedBufferData(renderer_id, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
	}

	IndexBuffer::~IndexBuffer() {
		GLCall(glDeleteBuffers(1, &renderer_id));
	}

	 void IndexBuffer::Bind() const {
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id));
	}
	 void IndexBuffer::Unbind() const {
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
}