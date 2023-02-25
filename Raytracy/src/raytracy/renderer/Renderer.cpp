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
	shared_ptr<ShaderProgram> shader_program;
	static const float clear_color[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	void Renderer::Init() {
		RTY_PROFILE_FUNCTION();
		RTY_RENDERER_ASSERT(!is_initialized, "Renderer is already initialized!");

		GLfloat  vertices[6][2] = {
		{ -0.90f, -0.90f }, {  0.85f, -0.90f }, { -0.90f,  0.85f },  // Triangle 1
		{  0.90f, -0.85f }, {  0.90f,  0.90f }, { -0.85f,  0.90f }   // Triangle 2
		};

		GLCall(glCreateVertexArrays(1, &vertex_array));

		GLCall(glCreateBuffers(1, &vertex_buffer));
		GLCall(glNamedBufferStorage(vertex_buffer, sizeof(vertices), vertices, NULL));

		GLCall(glBindVertexArray(vertex_array));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer));

		shader_program = make_shared<ShaderProgram>("Triangles");
		shader_program->Bind();

		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0));
		GLCall(glEnableVertexAttribArray(0));

		is_initialized = true;
	}


	void Renderer::Submit() {
		RTY_PROFILE_FUNCTION();
		RTY_RENDERER_ASSERT(is_initialized, "Renderer is not initialized!");
		Render();
	}

	void Renderer::Shutdown() {
		RTY_PROFILE_FUNCTION();
		GLCall(glDeleteVertexArrays(1, &vertex_array));
		GLCall(glDeleteBuffers(1, &vertex_buffer));
	}

	void Renderer::Render() {
		RTY_PROFILE_FUNCTION();
		GLCall(glClearBufferfv(GL_COLOR, 0, clear_color));


		GLCall(glBindVertexArray(vertex_array));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
	}
}