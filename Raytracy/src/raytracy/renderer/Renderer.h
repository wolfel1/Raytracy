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

	private:
		Renderer() {}

		void Render();
	};
}