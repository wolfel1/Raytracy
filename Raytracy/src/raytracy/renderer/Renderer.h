#pragma once

#include "api/RendererAPI.h"
#include "model/Mesh.h"
#include "camera/PerspectiveCamera.h"
#include "ViewportScene.h"

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

		void Submit(shared_ptr<renderer::Scene> const scene);

		void Shutdown();

		bool OnWindowResize(uint32_t width, uint32_t height);

	private:
		Renderer() {}

		void BeginScene(std::shared_ptr<PerspectiveCamera> camera);
		void Render();
		void EndScene();
	};
}