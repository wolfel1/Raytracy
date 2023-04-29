#include "raytracypch.h"
#include "Buffer.h"

#include "opengl/OpenGLBuffer.h"


namespace raytracy {
	shared_ptr<VertexBuffer> VertexBuffer::Create(size_t size) {
		return make_shared<OpenGLVertexBuffer>(size);
	}

	shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, size_t size) {
		return make_shared<OpenGLVertexBuffer>(vertices, size);
	}

	shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
		return make_shared<OpenGLIndexBuffer>(indices, count);
	}
}