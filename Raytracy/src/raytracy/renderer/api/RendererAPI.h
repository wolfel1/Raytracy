#pragma once

#include "Buffer.h"
#include "GraphicsContext.h"
namespace raytracy {
	class RendererAPI {
	public:
		enum class API {
			None = 0, OpenGL = 1, Vulkan = 2
		};
	private:

		static API graphics_api;

		protected:
		shared_ptr<GraphicsContext> graphics_context;

	public:
		static unique_ptr<RendererAPI> Create();

		static API GetAPI() { return graphics_api; }
		static void SetAPI(API api) { graphics_api = api; }

		virtual void Init(shared_ptr<GraphicsContext> context) = 0;

		virtual void ClearViewport() = 0;

		virtual void SetClearColor(const glm::vec4& clear_color) = 0;

		virtual void DrawIndexed(const shared_ptr<IndexBuffer>& index_buffer) = 0;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void Shutdown() = 0;
	};
}