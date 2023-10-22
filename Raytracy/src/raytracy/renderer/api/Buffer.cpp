#include "raytracypch.h"
#include "Buffer.h"

#include "RendererAPI.h"
#include "opengl/OpenGLBuffer.h"


namespace raytracy {
	shared_ptr<VertexBuffer> VertexBuffer::Create(size_t size) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:
			RTY_ASSERT(false, "Renderer API None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return make_shared<OpenGLVertexBuffer>(size);
		default:
			break;
		}

		RTY_ASSERT(false, "Unknown RendereAPI!");
		return nullptr;
	}

	shared_ptr<VertexBuffer> VertexBuffer::Create(std::vector<Vertex>& vertices) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:
			RTY_ASSERT(false, "Renderer API None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return make_shared<OpenGLVertexBuffer>(vertices);
		default:
			break;
		}

		RTY_ASSERT(false, "Unknown RendereAPI!");
		return nullptr;
	}

	shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:
			RTY_ASSERT(false, "Renderer API None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return make_shared<OpenGLIndexBuffer>(indices, count);
		default:
			break;
		}

		RTY_ASSERT(false, "Unknown RendereAPI!");
		return nullptr;
	}
	shared_ptr<UniformBuffer> UniformBuffer::Create(const BufferLayout& layout) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:
			RTY_ASSERT(false, "Renderer API None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return make_shared<OpenGLUniformBuffer>(layout);
		default:
			break;
		}

		RTY_ASSERT(false, "Unknown RendereAPI!");
		return nullptr;
	}
}