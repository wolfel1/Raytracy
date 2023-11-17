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
		
		camera_uniform_buffer = UniformBuffer::Create("Camera", {
			{ "model", VertexDataType::Mat4 },
			{ "view", VertexDataType::Mat4 },
			{ "projection", VertexDataType::Mat4 }
		});

		is_initialized = true;
	}

	void Renderer::BeginScene(PerspectiveCamera const& camera) {
		RTY_ASSERT(is_initialized, "Renderer is not initialized!");
		scene_data.meshes.clear();
		camera_uniform_buffer->SetMat4("view", camera.GetViewMatrix());
		camera_uniform_buffer->SetMat4("projection", camera.GetProjectionMatrix());
	}

	void Renderer::Submit(shared_ptr<Mesh> const mesh) {
		scene_data.meshes.push_back(mesh);
		camera_uniform_buffer->SetMat4("model", mesh->GetModelMatrix());
	}

	void Renderer::Render() {
		RTY_PROFILE_FUNCTION();
		renderer_api->ClearViewport();

		for (auto& mesh : scene_data.meshes) {
			auto vertex_array = mesh->GetVertexArray();
			auto shader = mesh->GetShader();

			vertex_array->Bind();
			shader->Bind();
			renderer_api->DrawIndexed(vertex_array);
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