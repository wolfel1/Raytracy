#include "raytracypch.h"
#include "Renderer.h"

#include "api/Shader.h"
#include "api/Buffer.h"
#include "api/VertexArray.h"
#include "api/GraphicsContext.h"
#include <GLFW/glfw3.h>


namespace raytracy {

	static const glm::vec4 clear_color = { 0.01f, 0.01f, 0.01f, 1.0f };

	RendererAPI::API RendererAPI::graphics_api = RendererAPI::API::OpenGL;

	void Renderer::Init() {
		RTY_PROFILE_FUNCTION();
		RTY_ASSERT(!is_initialized, "Renderer is already initialized!");

		renderer_api = RendererAPI::Create();
		renderer_api->Init();

		renderer_api->SetClearColor(clear_color);

		is_initialized = true;
	}

	void Renderer::BeginScene(PerspectiveCamera const& camera) {
		RTY_ASSERT(is_initialized, "Renderer is not initialized!");
		scene_data = {};
		scene_data.view_matrix = camera.GetViewMatrix();
		scene_data.projection_matrix = camera.GetProjectionMatrix();
	}

	void Renderer::Submit(shared_ptr<Mesh> const mesh) {
		scene_data.meshes.push_back(mesh);
	}

	void Renderer::Render() {
		RTY_PROFILE_FUNCTION();
		renderer_api->ClearViewport();

		for (auto mesh : scene_data.meshes) {
			auto vertex_array = mesh->GetVertexArray();
			auto shader = mesh->GetShader();
			auto& uniform_buffers = shader->GetUniformBuffers();
			auto it = uniform_buffers.find("camera");
			RTY_ASSERT(it != uniform_buffers.end(), "No uniform buffer with key 'camera' exists!");
			it->second->SetMat4("model", mesh->GetModelMatrix());
			it->second->SetMat4("view", scene_data.view_matrix);
			it->second->SetMat4("projection", scene_data.projection_matrix);

			vertex_array->Bind();
			shader->Bind();
			renderer_api->DrawIndexed(vertex_array, uniform_buffers);
		}
		
	}

	void Renderer::EndScene() {
		Render();
	}

	void Renderer::Shutdown() {
		RTY_PROFILE_FUNCTION();
	}

	bool Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		renderer_api->SetViewport(0, 0, width, height);
		return true;
	}
}