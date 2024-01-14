#include "raytracypch.h"
#include "OpenGLRendererAPI.h"

#include <glad/gl.h>

namespace raytracy {
	void GLClearError() {
		while (glGetError() != GL_NO_ERROR);
	}

	bool GLLogCall(const char* function, const char* file, int line) {
		while (GLenum error = glGetError()) {
			RTY_RENDERER_ERROR("[OpenGL Error] ({0}) in {1}, {2} at {3}", error, function, file, line);
			return false;
		}
		return true;
	}

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




	void OpenGLRendererAPI::Init() {
		RTY_PROFILE_FUNCTION();

#ifdef RTY_DEBUG
		GLCall(glEnable(GL_DEBUG_OUTPUT));
		GLCall(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
		GLCall(glDebugMessageCallback(OpenGLMessageCallback, nullptr));

		GLCall(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE));
#endif
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		glEnable(GL_FRAMEBUFFER_SRGB);

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LEQUAL));

		GLCall(glEnable(GL_CULL_FACE));
		GLCall(glCullFace(GL_BACK));
		GLCall(glFrontFace(GL_CCW));
	}

	void OpenGLRendererAPI::ClearViewport() {
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& clear_color) {
		GLCall(glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a));
	}

	void OpenGLRendererAPI::Draw(const shared_ptr<OpenGLVertexArray> vertex_array) {
		//InstrumentationTimer timer("Render", true);
		auto vertex_buffer = vertex_array->GetVertexBuffer();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, vertex_buffer->GetCount()));
	}

	void OpenGLRendererAPI::Draw(uint32_t count) {
		glDrawArrays(GL_TRIANGLE_STRIP, 0, count);
	}

	void OpenGLRendererAPI::LaunchComputeShader(uint32_t num_groups_x, uint32_t num_groups_y, uint32_t num_groups_z) {
		glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
	}

	void OpenGLRendererAPI::SetMemoryBarrier() {
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const shared_ptr<OpenGLVertexArray> vertex_array) {
		auto index_buffer = vertex_array->GetIndexBuffer();
		//InstrumentationTimer timer("Render", true);
		GLCall(glDrawElements(GL_TRIANGLES, index_buffer->GetCount(), GL_UNSIGNED_INT, (void*)0));
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		GLCall(glViewport(x, y, width, height));
	}

	void OpenGLRendererAPI::SetFrontFace(FrontFace side) {
		if (side == FrontFace::OUTSIDE) {
			GLCall(glFrontFace(GL_CCW));

		} else if (side == FrontFace::INSIDE) {
			GLCall(glFrontFace(GL_CW));
		} else {
			RTY_RENDERER_ERROR("Undefined side!");
		}
	}
}