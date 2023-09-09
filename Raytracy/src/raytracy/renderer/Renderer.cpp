#include "raytracypch.h"
#include "Renderer.h"

#include "api/Shader.h"
#include "api/Buffer.h"
#include "api/VertexArray.h"
#include "api/GraphicsContext.h"
#include <GLFW/glfw3.h>


namespace raytracy {

	shared_ptr<VertexArray> vertex_array;
	shared_ptr<VertexBuffer> vertex_buffer;
	shared_ptr<IndexBuffer> index_buffer;
	shared_ptr<Shader> shader_program;
	shared_ptr<UniformBuffer> uniform_buffer;
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


	void Renderer::Submit(shared_ptr<Mesh> const mesh) {
		RTY_ASSERT(is_initialized, "Renderer is not initialized!");
		Render(mesh);
	}

	void Renderer::Render(shared_ptr<Mesh> const mesh) {
		RTY_PROFILE_FUNCTION();

		auto vertex_array = mesh->GetVertexArray();
		auto shader = mesh->GetShader();
		auto uniform_buffer = mesh->GetUniformBuffer();

		renderer_api->ClearViewport();

		vertex_array->Bind();
		shader->Bind();
		renderer_api->DrawIndexed(vertex_array, uniform_buffer);
	}

	void Renderer::Shutdown() {
		RTY_PROFILE_FUNCTION();
	}

	bool Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		renderer_api->SetViewport(0, 0, width, height);
		return true;
	}
}