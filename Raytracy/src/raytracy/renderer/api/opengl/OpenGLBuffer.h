#pragma once

#include "../Buffer.h"

namespace raytracy {
	class OpenGLVertexBuffer : public VertexBuffer {

	public:
		OpenGLVertexBuffer(size_t size);
		OpenGLVertexBuffer(const std::vector<Vertex>& vertices);
		~OpenGLVertexBuffer();

		void Bind() const;
		void Unbind() const;
	};

	class OpenGLIndexBuffer : public IndexBuffer {

	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		~OpenGLIndexBuffer();

		void Bind() const;
		void Unbind() const;
	};
}