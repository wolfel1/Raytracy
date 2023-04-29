#include "raytracypch.h"
#include "OpenGLBuffer.h"

#include <glad/gl.h>

#include "OpenGLRendererAPI.h"

namespace raytracy {
	OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size) {
		GLCall(glCreateBuffers(1, &renderer_id));
		GLCall(glNamedBufferData(renderer_id, size, nullptr, GL_DYNAMIC_DRAW));
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, size_t size) {
		GLCall(glCreateBuffers(1, &renderer_id));
		GLCall(glNamedBufferData(renderer_id, size * sizeof(float), vertices, GL_STATIC_DRAW));
    }
	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		GLCall(glDeleteBuffers(1, &renderer_id));
	}
	 void OpenGLVertexBuffer::Bind() const {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, renderer_id));
	}
	 void OpenGLVertexBuffer::Unbind() const {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : count(count) {
		GLCall(glCreateBuffers(1, &renderer_id));
		GLCall(glNamedBufferData(renderer_id, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		GLCall(glDeleteBuffers(1, &renderer_id));
	}

	 void OpenGLIndexBuffer::Bind() const {
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id));
	}
	 void OpenGLIndexBuffer::Unbind() const {
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
}