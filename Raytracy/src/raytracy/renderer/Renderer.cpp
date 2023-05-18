#include "raytracypch.h"
#include "Renderer.h"

#include "api/Shader.h"
#include "api/Buffer.h"
#include "api/VertexArray.h"
#include "api/GraphicsContext.h"


namespace raytracy {

	shared_ptr<VertexArray> vertex_array;
	shared_ptr<VertexBuffer> vertex_buffer;
	shared_ptr<IndexBuffer> index_buffer;
	shared_ptr<Shader> shader_program;
	static const glm::vec4 clear_color = { 0.1f, 0.1f, 0.1f, 1.0f };

	RendererAPI::API RendererAPI::graphics_api = RendererAPI::API::Vulkan;

	void Renderer::Init(shared_ptr<GraphicsContext> context) {
		RTY_PROFILE_FUNCTION();
		RTY_ASSERT(!is_initialized, "Renderer is already initialized!");

		renderer_api = RendererAPI::Create();
		renderer_api->Init(context);

		renderer_api->SetClearColor(clear_color);

		float  vertices[] = {
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f, 1.0f,
			0.5f, 0.5f, 0.0f,     0.0f, 1.0f, 0.0f, 1.0f,
			-0.5f, 0.5f, 0.0f,    1.0f, 0.0f, 0.0f, 1.0f
		};

		uint32_t indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		vertex_array = VertexArray::Create();

		vertex_buffer = VertexBuffer::Create(vertices, sizeof(vertices));
		vertex_buffer->SetLayout({
			{ "position", VertexDataType::Float3 },
			{ "color", VertexDataType::Float4 }
		});
		//GLCall(glNamedBufferSubData(vertex_buffer, 0, sizeof(vertices), ));
		//GLCall(glNamedBufferSubData(vertex_buffer, sizeof(vertices), sizeof(vertex_colors), vertex_colors));

		index_buffer = IndexBuffer::Create(indices, 6);

		shader_program = ShaderLibrary::Get().Load("basic");
		RTY_ASSERT(shader_program, "Could not create a shader program!");

		vertex_array->SetVertexBuffer(vertex_buffer);
		vertex_array->SetIndexBuffer(index_buffer);

		is_initialized = true;
	}


	void Renderer::Submit() {
		RTY_ASSERT(is_initialized, "Renderer is not initialized!");
		Render();
	}

	void Renderer::Render() {
		RTY_PROFILE_FUNCTION();
		renderer_api->ClearViewport();

		vertex_array->Bind();
		shader_program->Bind();
		renderer_api->DrawIndexed(index_buffer);
	}

	void Renderer::Shutdown() {
		RTY_PROFILE_FUNCTION();
		vertex_array.reset();
		vertex_buffer.reset();
		index_buffer.reset();
	}

	bool Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		renderer_api->SetViewport(0, 0, width, height);
		return true;
	}
}