#pragma once

#include "api/opengl/OpenGLRendererAPI.h"
#include "model/Mesh.h"
#include "camera/PerspectiveCamera.h"
#include "ViewportScene.h"

namespace raytracy {

	enum class API {
		None = 0, OpenGL = 1, Vulkan = 2
	};

	class Renderer {
	private:

		static API graphics_api;

		struct SceneData {
			std::vector<shared_ptr<renderer::Mesh>> meshes;

			glm::mat4 view_matrix;
			glm::mat4 projection_matrix;
			shared_ptr<OpenGLUniformBuffer> camera_uniform_buffer;
		};

		SceneData scene_data{};

		bool is_initialized = false;

		shared_ptr<OpenGLRendererAPI> renderer_api = nullptr;

	public:
		Renderer(const Renderer&) = delete;

		static Renderer& Get() {
			static Renderer renderer;
			return renderer;
		}

		static API GetAPI() { return graphics_api; }
		static void SetAPI(API api) { graphics_api = api; }

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