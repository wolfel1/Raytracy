#include "raytracypch.h"
#include "Buffer.h"

#include "RendererAPI.h"
#include "opengl/OpenGLBuffer.h"
#include "vulkan/VulkanBuffer.h"


namespace raytracy {
	shared_ptr<VertexBuffer> VertexBuffer::Create(size_t size) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:
			RTY_ASSERT(false, "Renderer API None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return make_shared<OpenGLVertexBuffer>(size);
		case RendererAPI::API::Vulkan:
			return make_shared<VulkanVertexBuffer>(size);
		default:
			break;
		}

		RTY_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, size_t size) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:
			RTY_ASSERT(false, "Renderer API None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return make_shared<OpenGLVertexBuffer>(vertices, size);
		case RendererAPI::API::Vulkan:
			return make_shared<VulkanVertexBuffer>(vertices, size);
		default:
			break;
		}

		RTY_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:
			RTY_ASSERT(false, "Renderer API None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return make_shared<OpenGLIndexBuffer>(indices, count);
		case RendererAPI::API::Vulkan:
			return make_shared<VulkanIndexBuffer>(indices, count);
		default:
			break;
		}

		RTY_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}