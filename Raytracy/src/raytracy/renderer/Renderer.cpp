#include "raytracypch.h"
#include "Renderer.h"

#include <glad/gl.h>

#include "api/opengl/OpenGLRendererAPI.h"
#include "api/Shader.h"
#include "api/Buffer.h"


namespace raytracy {

	GLuint vertex_array;
	shared_ptr<VertexBuffer> vertex_buffer;
	shared_ptr<IndexBuffer> index_buffer;
	shared_ptr<ShaderProgram> shader_program;
	static const glm::vec4 clear_color = { 0.1f, 0.1f, 0.1f, 1.0f };

	RendererAPI::API RendererAPI::graphics_api = RendererAPI::API::OpenGL;

	void Renderer::Init() {
		RTY_PROFILE_FUNCTION();
		RTY_ASSERT(!is_initialized, "Renderer is already initialized!");

		renderer_api = RendererAPI::Create();
		renderer_api->Init();
		
		renderer_api->SetClearColor(clear_color);

		GLfloat  vertices[] = {
			-0.5f, -0.5f, 0.0f,		
			0.5f, -0.5f, 0.0f,		
			0.5f, 0.5f, 0.0f,		
			-0.5f, 0.5f, 0.0f,		

			0.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,	
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f		
		};	
		
		GLfloat vertex_colors[] = {
			0.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f
		};

		GLuint indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glCreateVertexArrays(1, &vertex_array));

		vertex_buffer = VertexBuffer::Create(vertices, sizeof(vertices));
		//GLCall(glNamedBufferSubData(vertex_buffer, 0, sizeof(vertices), ));
		//GLCall(glNamedBufferSubData(vertex_buffer, sizeof(vertices), sizeof(vertex_colors), vertex_colors));

		index_buffer = IndexBuffer::Create(indices, 6);

		shader_program = ShaderLibrary::Get().Load("basic");
		RTY_ASSERT(shader_program, "Could not create a shader program!");

		GLCall(glBindVertexArray(vertex_array));
		vertex_buffer->Bind();
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glEnableVertexAttribArray(1));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
		GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)(3* 4* sizeof(float))));

		is_initialized = true;
	}


	void Renderer::Submit() {
		RTY_ASSERT(is_initialized, "Renderer is not initialized!");
		Render();
	}

	void Renderer::Render() {
		RTY_PROFILE_FUNCTION();
		renderer_api->ClearViewport();

		GLCall(glBindVertexArray(vertex_array));
		index_buffer->Bind();
		shader_program->Bind();
		renderer_api->DrawIndexed(index_buffer);
	}

	void Renderer::Shutdown() {
		RTY_PROFILE_FUNCTION();
		GLCall(glDeleteVertexArrays(1, &vertex_array));
		vertex_buffer.reset();
		index_buffer.reset();
	}

	bool Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		renderer_api->SetViewport(0, 0, width, height);
		return true;
	}
}