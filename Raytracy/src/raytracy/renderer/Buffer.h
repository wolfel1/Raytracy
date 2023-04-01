#pragma once

namespace raytracy {
	class VertexBuffer {
	private:
		uint32_t renderer_id{};

	public:
		VertexBuffer(uint32_t size);
		VertexBuffer(float* vertices);
		~VertexBuffer();

		const void Bind() const;
		const void Unbind() const;
	};

	class IndexBuffer {
	private:
		uint32_t renderer_id{};

	public:
		IndexBuffer(uint32_t* indices, uint32_t count);
		~IndexBuffer();

		const void Bind() const;
		const void Unbind() const;
	};
}