#pragma once

namespace raytracy {
	class OpenGLVertexBuffer {
	private:
		uint32_t renderer_id{};

	public:
		OpenGLVertexBuffer(size_t size);
		OpenGLVertexBuffer(float* vertices, size_t size);
		~OpenGLVertexBuffer();

		void Bind() const;
		void Unbind() const;
	};

	class OpenGLIndexBuffer {
	private:
		uint32_t renderer_id{};
		uint32_t count{};

	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		~OpenGLIndexBuffer();

		uint32_t GetCount() { return count; }

		 void Bind() const;
		 void Unbind() const;
	};
}