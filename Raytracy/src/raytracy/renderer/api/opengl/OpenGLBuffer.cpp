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

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : IndexBuffer(count) {
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

	OpenGLUniformBuffer::OpenGLUniformBuffer(const BufferLayout& layout) : UniformBuffer(layout) {
		GLCall(glCreateBuffers(1, &renderer_id));
		glNamedBufferData(renderer_id, layout.GetStride(), NULL, GL_STATIC_DRAW);
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

	void OpenGLUniformBuffer::Link() const {
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, renderer_id);
	}

	void OpenGLUniformBuffer::SetVec4(const std::string& name, const glm::vec4& value) const {
		auto& elements = layout.GetElements();
		
		auto element = std::find_if(elements.begin(), elements.end(), [&name](const BufferElement& element) { return element.name == name; });
		RTY_ASSERT(element != elements.end(), "No uniform found with name {0}!", name);

		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, renderer_id));
		glBufferSubData(GL_UNIFORM_BUFFER, element->offset, SizeOfVertexDataType(element->type), glm::value_ptr(value));
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
	}
}