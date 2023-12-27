#include "raytracypch.h"
#include "Renderer.h"

#include "api/Shader.h"
#include "api/Buffer.h"
#include "api/VertexArray.h"
#include "api/GraphicsContext.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_inverse.hpp>


namespace raytracy {

	RendererAPI::API RendererAPI::graphics_api = RendererAPI::API::OpenGL;

	void Renderer::Init() {
		RTY_PROFILE_FUNCTION();
		RTY_ASSERT(!is_initialized, "Renderer is already initialized!");

		renderer_api = RendererAPI::Create();
		renderer_api->Init();

		renderer_api->SetClearColor({ 0.5f, 0.7f, 1.0f, 1.0f });

		is_initialized = true;
	}

	void Renderer::BeginScene(std::shared_ptr<PerspectiveCamera> camera) {
		RTY_ASSERT(is_initialized, "Renderer is not initialized!");
		scene_data.meshes.clear();
		scene_data.view_matrix = camera->GetViewMatrix();
		scene_data.projection_matrix = camera->GetProjectionMatrix();
		scene_data.camera_uniform_buffer = camera->GetCameraUniformBuffer();
	}

	void Renderer::Submit(shared_ptr<renderer::Scene> const scene) {
		BeginScene(scene->GetCamera());
		scene_data.meshes = scene->GetMeshes();
		EndScene();
	}

	void Renderer::EndScene() {
		Render();
	}

	void Renderer::Render() {
		RTY_PROFILE_FUNCTION();
		renderer_api->ClearViewport();

		for (auto& mesh : scene_data.meshes) {

			auto& model_matrix = mesh->GetModelMatrix();
			glm::mat4 model_view_matrix(scene_data.view_matrix * model_matrix);
			glm::mat4 model_view_projection_matrix(scene_data.projection_matrix * scene_data.view_matrix * model_matrix);
			auto normal_matrix = transpose(inverse(model_matrix));
			scene_data.camera_uniform_buffer->SetMat4("model_view_projection_matrix", model_view_projection_matrix);
			scene_data.camera_uniform_buffer->SetMat4("model_view_matrix", model_view_matrix);
			scene_data.camera_uniform_buffer->SetMat4("normal_matrix", normal_matrix);

			mesh->Draw(renderer_api);
		}

	}


	void Renderer::Shutdown() {
		RTY_PROFILE_FUNCTION();
	}

	bool Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		renderer_api->SetViewport(0, 0, width, height);
		return true;
	}
}