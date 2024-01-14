#include "raytracypch.h"
#include "OpenGLBuffer.h"

#include <glad/gl.h>

#include "OpenGLRendererAPI.h"
#include "../../Renderer.h"
#include "glm/gtc/type_ptr.hpp"

namespace raytracy {
	OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size) {
		GLCall(glCreateBuffers(1, &renderer_id));
		GLCall(glNamedBufferData(renderer_id, size, nullptr, GL_DYNAMIC_DRAW));
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(const std::vector<Vertex>& vertices) {
		GLCall(glCreateBuffers(1, &renderer_id));
		GLCall(glNamedBufferData(renderer_id, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW));
		count = static_cast<uint32_t>(vertices.size());
    }
	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		GLCall(glDeleteBuffers(1, &renderer_id));
	}
	 void OpenGLVertexBuffer::Bind() const {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, renderer_id));
	}
	 void OpenGLVertexBuffer::Unbind() const {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : count(count) {
		GLCall(glCreateBuffers(1, &renderer_id));
		GLCall(glNamedBufferData(renderer_id, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		GLCall(glDeleteBuffers(1, &renderer_id));
	}

	void OpenGLIndexBuffer::Bind() const {
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id));
	}

	void OpenGLIndexBuffer::Unbind() const {
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	OpenGLUniformBuffer::OpenGLUniformBuffer(std::string const& name, const BufferLayout& layout) : name(name), layout(layout) {
		GLCall(glCreateBuffers(1, &renderer_id));
		glNamedBufferData(renderer_id, layout.GetStride(), NULL, GL_DYNAMIC_COPY);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer() {
		GLCall(glDeleteBuffers(1, &renderer_id));
	}

	void OpenGLUniformBuffer::Bind() const {
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, renderer_id));
	}

	void OpenGLUniformBuffer::Unbind() const {
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
	}

	void OpenGLUniformBuffer::Link(uint32_t const index) const {
		GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, index, renderer_id));
	}

	void OpenGLUniformBuffer::SetFloat(const std::string& name, const float value) const {
		auto& elements = layout.GetElements();

		auto element = std::find_if(elements.begin(), elements.end(), [&name](const BufferElement& element) { return element.name == name; });
		RTY_ASSERT(element != elements.end(), "No uniform found with name {0}!", name);

		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, renderer_id));
		GLCall(glBufferSubData(GL_UNIFORM_BUFFER, element->offset, SizeOfVertexDataType(element->type), &value));
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
	}

	void OpenGLUniformBuffer::SetInt(const std::string& name, const int value) const {
		auto& elements = layout.GetElements();

		auto element = std::find_if(elements.begin(), elements.end(), [&name](const BufferElement& element) { return element.name == name; });
		RTY_ASSERT(element != elements.end(), "No uniform found with name {0}!", name);

		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, renderer_id));
		GLCall(glBufferSubData(GL_UNIFORM_BUFFER, element->offset, SizeOfVertexDataType(element->type), &value));
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
	}

	void OpenGLUniformBuffer::SetVec3(const std::string& name, const glm::vec3& value) const {
		auto& elements = layout.GetElements();

		auto element = std::find_if(elements.begin(), elements.end(), [&name](const BufferElement& element) { return element.name == name; });
		RTY_ASSERT(element != elements.end(), "No uniform found with name {0}!", name);

		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, renderer_id));
		GLCall(glBufferSubData(GL_UNIFORM_BUFFER, element->offset, SizeOfVertexDataType(element->type), glm::value_ptr(value)));
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
	}

	void OpenGLUniformBuffer::SetVec4(const std::string& name, const glm::vec4& value) const {
		auto& elements = layout.GetElements();
		
		auto element = std::find_if(elements.begin(), elements.end(), [&name](const BufferElement& element) { return element.name == name; });
		RTY_ASSERT(element != elements.end(), "No uniform found with name {0}!", name);

		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, renderer_id));
		GLCall(glBufferSubData(GL_UNIFORM_BUFFER, element->offset, SizeOfVertexDataType(element->type), glm::value_ptr(value)));
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
	}

	void OpenGLUniformBuffer::SetMat3(const std::string& name, const glm::mat3& value) const {
		auto& elements = layout.GetElements();

		auto element = std::find_if(elements.begin(), elements.end(), [&name](const BufferElement& element) { return element.name == name; });
		RTY_ASSERT(element != elements.end(), "No uniform found with name {0}!", name);

		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, renderer_id));
		GLCall(glBufferSubData(GL_UNIFORM_BUFFER, element->offset, SizeOfVertexDataType(element->type), glm::value_ptr(value)));
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
	}

	void OpenGLUniformBuffer::SetMat4(const std::string& name, const glm::mat4& value) const {
		auto& elements = layout.GetElements();

		auto element = std::find_if(elements.begin(), elements.end(), [&name](const BufferElement& element) { return element.name == name; });
		RTY_ASSERT(element != elements.end(), "No uniform found with name {0}!", name);

		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, renderer_id));
		GLCall(glBufferSubData(GL_UNIFORM_BUFFER, element->offset, SizeOfVertexDataType(element->type), glm::value_ptr(value)));
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
	}

	OpenGLStorageBuffer::OpenGLStorageBuffer(std::string const& name, size_t size) : name(name) {
		glCreateBuffers(1, &renderer_id);
		glNamedBufferData(renderer_id, size, NULL, GL_DYNAMIC_COPY);
	}

	OpenGLStorageBuffer::~OpenGLStorageBuffer() {
		GLCall(glDeleteBuffers(1, &renderer_id));
	}

	void OpenGLStorageBuffer::BindSlot(uint32_t slot) const {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, renderer_id);
	}

	void OpenGLStorageBuffer::Bind() const {
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, renderer_id));
	}

	void OpenGLStorageBuffer::Unbind() const {
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
	}

	void OpenGLStorageBuffer::SetData(size_t size, void* data) const {
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, renderer_id));
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, data);
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
	}
}