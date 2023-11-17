#pragma once

#include "api/RendererAPI.h"
#include "model/Mesh.h"
#include "camera/PerspectiveCamera.h"

namespace raytracy {

	
	class Renderer {
	private:

		struct SceneData {
			std::vector<shared_ptr<Mesh>> meshes;

		};

		SceneData scene_data{};

		bool is_initialized = false;

		shared_ptr<RendererAPI> renderer_api = nullptr;

		shared_ptr<UniformBuffer> camera_uniform_buffer;

	public:
		Renderer(const Renderer&) = delete;

		static Renderer& Get() {
			static Renderer renderer;
			return renderer;
		}

		void Init();

		shared_ptr<UniformBuffer> GetCameraUniformBuffer() const {
			return camera_uniform_buffer;
		}

		void BeginScene(PerspectiveCamera const& camera);
		void Submit(shared_ptr<Mesh> const mesh);
		void EndScene();

		void Shutdown();

		bool OnWindowResize(uint32_t width, uint32_t height);

	private:
		Renderer() {}

		void Render();
	};
}