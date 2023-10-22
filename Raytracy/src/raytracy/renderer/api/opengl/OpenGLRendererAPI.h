#pragma once

#include "../RendererAPI.h"
#include "../Buffer.h"

namespace raytracy {

#ifdef RTY_DEBUG
#define GLCall(x) x;GLLogCall(#x, __FILE__, __LINE__);
	void GLClearError();

	bool GLLogCall(const char* function, const char* file, int line);
#else
#define GLCall(x) x
#endif

	class OpenGLRendererAPI : public RendererAPI {

	public:

		virtual void Init() override;

		virtual void ClearViewport() override;

		virtual void SetClearColor(const glm::vec4& clear_color) override;

		virtual void DrawIndexed(const shared_ptr<VertexArray> vertex_array, std::unordered_map<std::string, shared_ptr<UniformBuffer>> const& uniform_buffer) override;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void Shutdown() override {}
	};
}