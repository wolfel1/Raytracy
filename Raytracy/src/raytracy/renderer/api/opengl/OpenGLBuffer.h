#pragma once

#include "../Buffer.h"

namespace raytracy {
	class OpenGLVertexBuffer {
	private:
		uint32_t renderer_id{};

		BufferLayout layout;
		uint32_t count{};

	public:
		OpenGLVertexBuffer(size_t size);
		OpenGLVertexBuffer(const std::vector<Vertex>& vertices);
		~OpenGLVertexBuffer();

		static shared_ptr<OpenGLVertexBuffer> Create(size_t size) {
			return make_shared<OpenGLVertexBuffer>(size);
		}
		static shared_ptr<OpenGLVertexBuffer> Create(std::vector<Vertex>& vertices) {
			return make_shared<OpenGLVertexBuffer>(vertices);
		}

		void Bind() const;
		void Unbind() const;

		const BufferLayout& GetLayout() const {
			return layout;
		}
		void SetLayout(const BufferLayout& layout) {
			this->layout = layout;
		}
		uint32_t GetCount() { return count; }
	};

	class OpenGLIndexBuffer {
	private:
		uint32_t renderer_id{};
		uint32_t count{};

	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		~OpenGLIndexBuffer();
		static shared_ptr<OpenGLIndexBuffer> Create(uint32_t* indices, uint32_t count) {
			return make_shared<OpenGLIndexBuffer>(indices, count);
		}

		void Bind() const;
		void Unbind() const;

		uint32_t GetCount() { return count; }
	};

	class OpenGLUniformBuffer {
	private:
		uint32_t renderer_id{};
		BufferLayout layout;
		std::string name;

	public:
		OpenGLUniformBuffer(std::string const& name, const BufferLayout& layout);
		~OpenGLUniformBuffer();

		static shared_ptr<OpenGLUniformBuffer> Create(std::string const& name, const BufferLayout& layout) {
			return make_shared<OpenGLUniformBuffer>(name, layout);
		}

		const BufferLayout& GetLayout() const {
			return layout;
		}

		const std::string& GetName() const { return name; }
		const uint32_t GetID() const { return renderer_id; }

		void Bind() const;
		void Unbind() const;

		void Link(uint32_t const index) const;

		void SetFloat(const std::string& name, const float value) const;
		void SetVec3(const std::string& name, const glm::vec3& value) const;
		void SetVec4(const std::string& name, const glm::vec4& value) const;
		void SetMat3(const std::string& name, const glm::mat3& value) const;
		void SetMat4(const std::string& name, const glm::mat4& value) const;

	};
}