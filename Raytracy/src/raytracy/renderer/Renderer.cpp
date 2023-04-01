#include "raytracypch.h"
#include "Renderer.h"

#include <glad/gl.h>

#include "RendererAPI.h"
#include "Shader.h"
#include "Buffer.h"


namespace raytracy {

	GLuint vertex_array;
	shared_ptr<VertexBuffer> vertex_buffer;
	shared_ptr<IndexBuffer> index_buffer;
	shared_ptr<ShaderProgram> shader_program;
	static const glm::vec4 clear_color = { 0.1f, 0.1f, 0.1f, 1.0f };

	void OpenGLMessageCallback(
			unsigned source,
			unsigned type,
			unsigned id,
			unsigned severity,
			int length,
			const char* message,
			const void* userParam) {
		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:         RTY_RENDERER_CRITICAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       RTY_RENDERER_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:          RTY_RENDERER_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: RTY_RENDERER_TRACE(message); return;
		}

		RTY_ASSERT(false, "Unknown severity level!");
	}

	void Renderer::Init() {
		RTY_PROFILE_FUNCTION();
		RTY_ASSERT(!is_initialized, "Renderer is already initialized!");

#ifdef RTY_DEBUG
		GLCall(glEnable(GL_DEBUG_OUTPUT));
		GLCall(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
		GLCall(glDebugMessageCallback(OpenGLMessageCallback, nullptr));

		GLCall(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE));
#endif
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LESS));
		
		GLCall(glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a));

		GLCall(glEnable(GL_CULL_FACE));
		GLCall(glCullFace(GL_BACK));
		GLCall(glFrontFace(GL_CCW));

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

		vertex_buffer = make_shared<VertexBuffer>(vertices, sizeof(vertices));
		//GLCall(glCreateBuffers(1, &vertex_buffer));
		//GLCall(glNamedBufferData(vertex_buffer, sizeof(vertices), vertices, GL_STATIC_DRAW));
		//GLCall(glNamedBufferSubData(vertex_buffer, 0, sizeof(vertices), ));
		//GLCall(glNamedBufferSubData(vertex_buffer, sizeof(vertices), sizeof(vertex_colors), vertex_colors));

		index_buffer = make_shared<IndexBuffer>(indices, 6);
		//GLCall(glCreateBuffers(1, &index_buffer));
		//GLCall(glNamedBufferData(index_buffer, sizeof(indices), indices, GL_STATIC_DRAW));

		shader_program = ShaderLibrary::Get().Load("basic");
		RTY_ASSERT(shader_program, "Could not create a shader program!");

		GLCall(glBindVertexArray(vertex_array));
//GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer));
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
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		GLCall(glBindVertexArray(vertex_array));
		index_buffer->Bind();
		shader_program->Bind();
		GLCall(glDrawElements(GL_TRIANGLES, index_buffer->GetCount(), GL_UNSIGNED_INT, (void*)0));
	}

	void Renderer::Shutdown() {
		RTY_PROFILE_FUNCTION();
		GLCall(glDeleteVertexArrays(1, &vertex_array));
		vertex_buffer.reset();
		index_buffer.reset();
	}

	bool Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		GLCall(glViewport(0, 0, width, height));
		return true;
	}
}