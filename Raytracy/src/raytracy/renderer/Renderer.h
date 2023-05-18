#pragma once

#include "api/vulkan/VulkanRendererAPI.h"

namespace raytracy {

	class Renderer {
	private:
		bool is_initialized = false;

		unique_ptr<VulkanRendererAPI> renderer_api = nullptr;
	public:
		Renderer(const Renderer&) = delete;

		static Renderer& Get() {
			static Renderer renderer;
			return renderer;
		}

		void Init(shared_ptr<VulkanContext> context);

		void Submit();

		void Shutdown(); 
		
		bool OnWindowResize(uint32_t width, uint32_t height);

	private:
		Renderer() {}

		void Render();
	};
}