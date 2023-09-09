#pragma once

#include "api/RendererAPI.h"
#include "model/Mesh.h"

namespace raytracy {

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

		void Submit(shared_ptr<Mesh> const mesh);

		void Shutdown();

		bool OnWindowResize(uint32_t width, uint32_t height);

	private:
		Renderer() {}

		void Render(shared_ptr<Mesh> const mesh);
	};
}