#include "raytracypch.h"
#include "Renderer.h"

#include <glad/gl.h>

#include "Shader.h"

#ifdef RTY_DEBUG
#define GLCall(x) GLClearError();x;GLLogCall(#x, __FILE__, __LINE__)
static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		RTY_RENDERER_ERROR("[OpenGL Error] ({0}) in {1}, {2} at {3}", error, function, file, line);
		return false;
	}
	return true;
}
#else
#define GLCall(x) x
#endif

namespace raytracy {

	GLuint vertex_array;
	GLuint vertex_buffer;
	GLuint vertex_color_buffer;
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

		GLfloat  vertices[] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5, 0.0f,
		};

		GLfloat vertex_colors[] = {
			0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f
		};

		GLCall(glCreateVertexArrays(1, &vertex_array));

		GLCall(glCreateBuffers(1, &vertex_buffer));
		GLCall(glNamedBufferStorage(vertex_buffer, sizeof(vertices), vertices, NULL));

		GLCall(glCreateBuffers(1, &vertex_color_buffer));
		GLCall(glNamedBufferStorage(vertex_color_buffer, sizeof(vertex_colors), vertex_colors, NULL));


		shader_program = ShaderProgram::CreateFromDirectory("basic");
		RTY_ASSERT(shader_program, "Could not create a shader program!");
		shader_program->Bind();

		GLCall(glBindVertexArray(vertex_array));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_color_buffer));
		GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glEnableVertexAttribArray(1));

		GLCall(glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a));

		GLCall(glEnable(GL_CULL_FACE));
		GLCall(glCullFace(GL_BACK));
		GLCall(glFrontFace(GL_CCW));

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
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
	}

	void Renderer::Shutdown() {
		RTY_PROFILE_FUNCTION();
		GLCall(glDeleteVertexArrays(1, &vertex_array));
		GLCall(glDeleteBuffers(1, &vertex_buffer));
	}

	bool Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		GLCall(glViewport(0, 0, width, height));
		return true;
	}
}