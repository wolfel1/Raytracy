#include "raytracypch.h"
#include "OpenGLVertexArray.h"

#include <glad/gl.h>
#include "OpenGLRendererAPI.h"
#include "OpenGLBuffer.h"

namespace raytracy {

	static GLenum ConvertVertexDataTypeInOpenGLBaseType(VertexDataType type) {
		switch (type)
		{
		case raytracy::VertexDataType::Bool:
			return GL_BOOL;
		case raytracy::VertexDataType::Int:
		case raytracy::VertexDataType::Int2:
		case raytracy::VertexDataType::Int3:
		case raytracy::VertexDataType::Int4:
			return GL_INT;
		case raytracy::VertexDataType::Float:
		case raytracy::VertexDataType::Float2:
		case raytracy::VertexDataType::Float3:
		case raytracy::VertexDataType::Float4:
			return GL_FLOAT;
		}

		RTY_RENDERER_ERROR("Unknown Vertex Data Type!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray() {
		GLCall(glCreateVertexArrays(1, &renderer_id));
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		GLCall(glDeleteVertexArrays(1, &renderer_id));
	}

	void OpenGLVertexArray::Bind() {
		GLCall(glBindVertexArray(renderer_id));
	}

	void OpenGLVertexArray::Unbind() {
		GLCall(glBindVertexArray(0));
	}

	void OpenGLVertexArray::SetVertexBuffer(const shared_ptr<OpenGLVertexBuffer> vertex_buffer) {
		const auto& layout = vertex_buffer->GetLayout();
		RTY_ASSERT(layout.GetElements().size(), "Vertex buffer has no layout!");

		GLCall(glBindVertexArray(renderer_id));
		auto opengl_vertex_buffer = std::dynamic_pointer_cast<OpenGLVertexBuffer>(vertex_buffer);
		RTY_ASSERT(opengl_vertex_buffer, "Vertex buffer suits not to opengl!");
		opengl_vertex_buffer->Bind();

		const auto& elements = layout.GetElements();
		for (unsigned int i = 0; i < elements.size(); i++) {
			GLCall(glEnableVertexAttribArray(i));
			GLCall(glVertexAttribPointer(i,
										 elements[i].GetComponentCount(),
										 ConvertVertexDataTypeInOpenGLBaseType(elements[i].type),
										 elements[i].normalized ? GL_TRUE : GL_FALSE,
										 layout.GetStride(),
										 (const void*) static_cast<uintptr_t>(elements[i].offset)
			));
		}
		this->vertex_buffer = vertex_buffer;
	}

	void OpenGLVertexArray::SetIndexBuffer(const shared_ptr<OpenGLIndexBuffer> index_buffer) {
		GLCall(glBindVertexArray(renderer_id));
		auto opengl_index_buffer = std::dynamic_pointer_cast<OpenGLIndexBuffer>(index_buffer);
		RTY_ASSERT(opengl_index_buffer, "Index buffer suits not to opengl!");
		opengl_index_buffer->Bind();

		this->index_buffer = index_buffer;
	}
}
