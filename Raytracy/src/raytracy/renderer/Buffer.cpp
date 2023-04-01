#include "raytracypch.h"
#include "Buffer.h"

#include <glad/gl.h>

#include "RendererAPI.h"

namespace raytracy {
	VertexBuffer::VertexBuffer(uint32_t size) {
		GLCall(glCreateBuffers(1, &renderer_id));
		GLCall(glNamedBufferData(renderer_id, size, nullptr, GL_DYNAMIC_DRAW));
	}

	VertexBuffer::VertexBuffer(float* vertices) {
		GLCall(glCreateBuffers(1, &renderer_id));
		GLCall(glNamedBufferData(renderer_id, sizeof(vertices), vertices, GL_STATIC_DRAW));
    }
	VertexBuffer::~VertexBuffer() {
		GLCall(glDeleteBuffers(1, &renderer_id));
	}
	const void VertexBuffer::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
	}
	const void VertexBuffer::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) {
		GLCall(glCreateBuffers(1, &renderer_id));
		GLCall(glNamedBufferStorage(renderer_id, sizeof(indices), indices, GL_STATIC_DRAW));
	}

	IndexBuffer::~IndexBuffer() {
		GLCall(glDeleteBuffers(1, &renderer_id));
	}

	const void IndexBuffer::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id);
	}
	const void IndexBuffer::Unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}