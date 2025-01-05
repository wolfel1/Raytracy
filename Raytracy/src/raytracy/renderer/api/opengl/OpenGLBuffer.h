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
		static shared_ptr<OpenGLVertexBuffer> Create(std::vector<Vertex> const& vertices) {
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
		OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);
		~OpenGLIndexBuffer();
		static shared_ptr<OpenGLIndexBuffer> Create(const uint32_t* indices, uint32_t count) {
			return make_shared<OpenGLIndexBuffer>(indices, count);
		}

		void Bind() const;
		void Unbind() const;

		uint32_t GetCount() { return count; }
	};

	class OpenGLUniformBuffer {
	private:
		uint32_t renderer_id{};
		uint32_t block_index{};
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

		void SetBlockIndex(uint32_t index) {
			block_index = index;
		}

		uint32_t GetBlockIndex() const {
			return block_index;
		}

		const std::string& GetName() const { return name; }
		const uint32_t GetID() const { return renderer_id; }

		void Bind() const;
		void Unbind() const;

		void Link() const;

		void SetFloat(const std::string& name, const float value) const;
		void SetInt(const std::string& name, const int value) const;
		void SetVec3(const std::string& name, const glm::vec3& value) const;
		void SetVec4(const std::string& name, const glm::vec4& value) const;
		void SetMat3(const std::string& name, const glm::mat3& value) const;
		void SetMat4(const std::string& name, const glm::mat4& value) const;

	};

	class OpenGLStorageBuffer {
	private:
		uint32_t renderer_id{};
		std::string name;

		uint32_t size;

	public:
		OpenGLStorageBuffer(std::string const& name, size_t size = 8192);
		~OpenGLStorageBuffer();

		static shared_ptr<OpenGLStorageBuffer> Create(std::string const& name) {
			return make_shared<OpenGLStorageBuffer>(name);
		}

		void Bind() const; 
		void BindSlot(uint32_t slot) const;
		void Unbind() const;

		void SetData(size_t size, void* data);
	};
}