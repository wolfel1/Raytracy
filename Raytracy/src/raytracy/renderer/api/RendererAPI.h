#pragma once

#include "GraphicsContext.h"
#include "VertexArray.h"
#include "Shader.h"

namespace raytracy {
	class RendererAPI {
	public:
		enum class API {
			None = 0, OpenGL = 1, Vulkan = 2
		};
	private:

		static API graphics_api;

	public:
		static shared_ptr<RendererAPI> Create();

		static API GetAPI() { return graphics_api; }
		static void SetAPI(API api) { graphics_api = api; }

		virtual void Init() = 0;

		virtual void ClearViewport() = 0;

		virtual void SetClearColor(const glm::vec4& clear_color) = 0;

		virtual void DrawIndexed(const shared_ptr<VertexArray>& vertex_array) = 0;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void Shutdown() = 0;
	};
}