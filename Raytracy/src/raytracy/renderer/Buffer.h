#pragma once

namespace raytracy {
	class VertexBuffer {
	private:
		uint32_t renderer_id{};

	public:
		VertexBuffer(size_t size);
		VertexBuffer(float* vertices, size_t size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;
	};

	class IndexBuffer {
	private:
		uint32_t renderer_id{};
		uint32_t count{};

	public:
		IndexBuffer(uint32_t* indices, uint32_t count);
		~IndexBuffer();

		uint32_t GetCount() { return count; }

		 void Bind() const;
		 void Unbind() const;
	};
}