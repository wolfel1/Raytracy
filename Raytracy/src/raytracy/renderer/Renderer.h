#pragma once

#include "api/RendererAPI.h"
#include "model/Mesh.h"
#include "camera/PerspectiveCamera.h"

namespace raytracy {

	
	class Renderer {
	private:

		struct SceneData {
			std::vector<shared_ptr<renderer::Mesh>> meshes;

			glm::mat4 view_matrix;
			glm::mat4 projection_matrix;
			shared_ptr<UniformBuffer> camera_uniform_buffer;
		};

		SceneData scene_data{};

		bool is_initialized = false;

		shared_ptr<RendererAPI> renderer_api = nullptr;

	public:
		Renderer(const Renderer&) = delete;

		static Renderer& Get() {
			static Renderer renderer;
			return renderer;
		}

		void Init();

		void BeginScene(std::shared_ptr<PerspectiveCamera> camera);
		void Submit(shared_ptr<renderer::Mesh> const mesh);
		void EndScene();

		void Shutdown();

		bool OnWindowResize(uint32_t width, uint32_t height);

	private:
		Renderer() {}

		void Render();
	};
}