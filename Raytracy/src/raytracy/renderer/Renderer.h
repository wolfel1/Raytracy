#pragma once

namespace raytracy {

	class Renderer {
	private:
		bool is_initialized = false;
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