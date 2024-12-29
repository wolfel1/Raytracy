#include "raytracypch.h"
#include "Renderer.h"

#include "api/Shader.h"
#include "api/Buffer.h"

#if RAYTRACING
#include "raytracer/Raytracer.h"
#endif

#include <glad/gl.h>
#include <glm/gtc/matrix_inverse.hpp>


namespace raytracy {

	API Renderer::graphics_api = API::OpenGL;

	void Renderer::Init() {
		RTY_PROFILE_FUNCTION();
		RTY_ASSERT(!is_initialized, "Renderer is already initialized!");

		renderer_api = OpenGLRendererAPI::Create();
		renderer_api->Init();

		renderer_api->SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

#if RAYTRACING
		Raytracer::Get().Init(renderer_api);
#endif

		is_initialized = true;
	}

	void Renderer::BeginScene(std::shared_ptr<PerspectiveCamera> camera) {
		RTY_PROFILE_FUNCTION();
		RTY_ASSERT(is_initialized, "Renderer is not initialized!");
		scene_data.meshes.clear();
		scene_data.view_matrix = camera->GetViewMatrix();
		scene_data.projection_matrix = camera->GetProjectionMatrix();
		scene_data.camera_uniform_buffer = camera->GetCameraUniformBuffer();
	}

	void Renderer::Submit(shared_ptr<renderer::Scene> const scene) {
		RTY_PROFILE_FUNCTION();
		if (raytrace) {
#if RAYTRACING
			Raytracer::Get().Raytrace(scene);
#else
			RTY_RENDERER_ERROR("Raytracing is not supported in this build!");
#endif
		} else {
			BeginScene(scene->GetCamera());

			scene_data.meshes = scene->GetMeshes();

			if (auto skybox = scene->GetSkybox()) {
				scene_data.meshes.push_back(skybox);
			}
			EndScene();
		}
	}

	void Renderer::EndScene() {
		Render();
	}

	void Renderer::Render() {
		RTY_PROFILE_FUNCTION();
		renderer_api->ClearViewport();
		glm::mat4 view_projection_matrix(scene_data.projection_matrix * glm::mat4(glm::mat3(scene_data.view_matrix)));
		scene_data.camera_uniform_buffer->SetMat4("view_projection_matrix", view_projection_matrix);


		for (auto& mesh : scene_data.meshes) {

			auto& model_matrix = mesh->GetModelMatrix();
			glm::mat4 model_view_projection_matrix(scene_data.projection_matrix * scene_data.view_matrix * model_matrix);
			auto normal_matrix = transpose(inverse(model_matrix));
			scene_data.camera_uniform_buffer->SetMat4("model_view_projection_matrix", model_view_projection_matrix);
			scene_data.camera_uniform_buffer->SetMat4("model_matrix", model_matrix);
			scene_data.camera_uniform_buffer->SetMat4("normal_matrix", normal_matrix);

			mesh->Draw(renderer_api);
		}

	}


	void Renderer::Shutdown() {
		RTY_PROFILE_FUNCTION();
		renderer_api = nullptr;
		scene_data.meshes.clear();
		scene_data.camera_uniform_buffer = nullptr;

#if RAYTRACING
		Raytracer::Get().Shutdown();
#endif
	}

	bool Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		renderer_api->SetViewport(0, 0, width, height);

		return true;
	}
}