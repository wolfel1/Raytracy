#pragma once

#include "OpenGLBuffer.h"
namespace raytracy {
#ifdef RTY_DEBUG
#define GLCall(x) GLClearError();x;GLLogCall(#x, __FILE__, __LINE__)
	void GLClearError();

	bool GLLogCall(const char* function, const char* file, int line);
#else
#define GLCall(x) x
#endif

	class OpenGLRendererAPI {
	public:
		enum class API {
			None = 0, OpenGL = 1, Vulkan = 2
		};
	private:

		static API graphics_api;

	public:

		void Init();

		void ClearViewport();

		void SetClearColor(const glm::vec4& clear_color);

		void DrawIndexed(const shared_ptr<OpenGLIndexBuffer>& index_buffer);

		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		void Shutdown() {}
	};
}