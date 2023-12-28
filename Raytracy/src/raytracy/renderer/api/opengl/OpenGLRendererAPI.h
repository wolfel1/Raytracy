#pragma once

#include "OpenGLVertexArray.h"

namespace raytracy {

#ifdef RTY_DEBUG
#define GLCall(x) x;GLLogCall(#x, __FILE__, __LINE__);
	void GLClearError();

	bool GLLogCall(const char* function, const char* file, int line);
#else
#define GLCall(x) x
#endif
		

	class OpenGLRendererAPI {

	public:
		virtual ~OpenGLRendererAPI() = default;


		void Init();

		void ClearViewport();

		 void SetClearColor(const glm::vec4& clear_color) ;

		 void DrawIndexed(const shared_ptr<OpenGLVertexArray> vertex_array) ;
		 void Draw(const shared_ptr<OpenGLVertexArray> vertex_array) ;

		 void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) ;

		virtual void Shutdown() {}

		static shared_ptr<OpenGLRendererAPI> Create() {
			return make_shared< OpenGLRendererAPI>();
		}
	};
}