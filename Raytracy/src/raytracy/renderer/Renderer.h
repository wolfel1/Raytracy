#pragma once

#include "api/RendererAPI.h"

namespace raytracy {

	class Renderer {
	private:
		bool is_initialized = false;

		unique_ptr<RendererAPI> renderer_api = nullptr;
	public:
		Renderer(const Renderer&) = delete;

		static Renderer& Get() {
			static Renderer renderer;
			return renderer;
		}

		void Init(shared_ptr<GraphicsContext> context);

		void Submit();

		void Shutdown(); 
		
		bool OnWindowResize(uint32_t width, uint32_t height);

	private:
		Renderer() {}

		void Render();
	};
}