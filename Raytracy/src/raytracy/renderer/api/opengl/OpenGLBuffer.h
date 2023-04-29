#pragma once

#include "../Buffer.h"

namespace raytracy {
	class OpenGLVertexBuffer : public VertexBuffer {
	private:
		uint32_t renderer_id{};

	public:
		OpenGLVertexBuffer(size_t size);
		OpenGLVertexBuffer(float* vertices, size_t size);
		~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	private:
		uint32_t renderer_id{};
		uint32_t count{};

	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
	};
}