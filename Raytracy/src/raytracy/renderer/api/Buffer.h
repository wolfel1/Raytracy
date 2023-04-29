#pragma once

namespace raytracy {
	class VertexBuffer {
	protected:
		uint32_t renderer_id{};

	public:
		virtual ~VertexBuffer() = default;

		static shared_ptr<VertexBuffer> Create(size_t size);
		static shared_ptr<VertexBuffer> Create(float* vertices, size_t size);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};

	class IndexBuffer {
	protected:
		uint32_t renderer_id{};
		uint32_t count{};

	public:
		static shared_ptr<IndexBuffer> Create(uint32_t* indices, uint32_t count);
		virtual ~IndexBuffer() = default;

		uint32_t GetCount() { return count; }

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};
}