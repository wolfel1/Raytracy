#include "raytracypch.h"
#include "Renderer.h"

#include <glad/gl.h>

#include "RendererAPI.h"
#include "Shader.h"



namespace raytracy {

	GLuint vertex_array;
	GLuint vertex_buffer;
	GLuint index_buffer;
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
			-0.5f, 0.5f, 0.0f
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

		GLCall(glCreateBuffers(1, &vertex_buffer));
		GLCall(glNamedBufferStorage(vertex_buffer, sizeof(vertices), vertices, NULL));
		//GLCall(glNamedBufferSubData(vertex_buffer, 0, sizeof(vertices), ));
		//GLCall(glNamedBufferSubData(vertex_buffer, sizeof(vertices), sizeof(vertex_colors), vertex_colors));

		GLCall(glCreateBuffers(1, &index_buffer));
		GLCall(glNamedBufferStorage(index_buffer, sizeof(indices), indices, NULL));

		shader_program = ShaderLibrary::Get().Load("basic");
		RTY_ASSERT(shader_program, "Could not create a shader program!");

		GLCall(glBindVertexArray(vertex_array));

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer));
		GLCall(glEnableVertexAttribArray(0));
		//GLCall(glEnableVertexAttribArray(1));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
		//GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(vertices)));

		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer));

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		GLCall(glBindVertexArray(0));

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
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer));
		shader_program->Bind();
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0));
	}

	void Renderer::Shutdown() {
		RTY_PROFILE_FUNCTION();
		GLCall(glDeleteVertexArrays(1, &vertex_array));
		GLCall(glDeleteBuffers(1, &vertex_buffer));
		GLCall(glDeleteBuffers(1, &index_buffer));
	}

	bool Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		GLCall(glViewport(0, 0, width, height));
		return true;
	}
}