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

	class OpenGLUniformBuffer : public UniformBuffer {

	public:
		OpenGLUniformBuffer(const BufferLayout& layout);
		~OpenGLUniformBuffer();

		void Bind() const;
		void Unbind() const;

		virtual void Link() const override;

		virtual void SetVec4(const std::string& name, const glm::vec4& value) const override;
	};
}