#include "raytracypch.h"
#include "Buffer.h"


namespace raytracy {
	shared_ptr<VertexBuffer> VertexBuffer::Create(size_t size) {
		return shared_ptr<VertexBuffer>();
	}

	shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, size_t size) {
		return shared_ptr<VertexBuffer>();
	}

	shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
		return shared_ptr<IndexBuffer>();
	}
}