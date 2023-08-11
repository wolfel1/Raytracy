#pragma once

#include "api/RendererAPI.h"

namespace raytracy {

	struct Vertex {
		glm::vec3 position;
		glm::vec4 color;
	};

	class Renderer {
	private:
		bool is_initialized = false;

		shared_ptr<RendererAPI> renderer_api = nullptr;
	public:
		Renderer(const Renderer&) = delete;

		static Renderer& Get() {
			static Renderer renderer;
			return renderer;
		}

		void Init();

		void Submit();

		void Shutdown();

		bool OnWindowResize(uint32_t width, uint32_t height);

	private:
		Renderer() {}

		void Render();
	};
}