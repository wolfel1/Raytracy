#include "raytracypch.h"
#include "Buffer.h"

#include "RendererAPI.h"
#include "vulkan/VulkanBuffer.h"
#include "opengl/OpenGLBuffer.h"


namespace raytracy {
	shared_ptr<VertexBuffer> VertexBuffer::Create(size_t size, const shared_ptr<RendererAPI> api) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:
			RTY_ASSERT(false, "Renderer API None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return make_shared<OpenGLVertexBuffer>(size);
		case RendererAPI::API::Vulkan:
			return make_shared<VulkanVertexBuffer>(size, api);
		default:
			break;
		}

		RTY_ASSERT(false, "Unknown RendereAPI!");
		return nullptr;
	}

	shared_ptr<VertexBuffer> VertexBuffer::Create(std::vector<Vertex>& vertices, const shared_ptr<RendererAPI> api) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:
			RTY_ASSERT(false, "Renderer API None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return make_shared<OpenGLVertexBuffer>(vertices);
		case RendererAPI::API::Vulkan:
			return make_shared<VulkanVertexBuffer>(vertices, api);
		default:
			break;
		}

		RTY_ASSERT(false, "Unknown RendereAPI!");
		return nullptr;
	}

	shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count, const shared_ptr<RendererAPI> api) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:
			RTY_ASSERT(false, "Renderer API None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return make_shared<OpenGLIndexBuffer>(indices, count);
		case RendererAPI::API::Vulkan:
			return make_shared<VulkanIndexBuffer>(indices, count, api);
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
		case RendererAPI::API::Vulkan:
			return make_shared<VulkanUniformBuffer>(layout);
		default:
			break;
		}

		RTY_ASSERT(false, "Unknown RendereAPI!");
		return nullptr;
	}
}