#pragma once

namespace raytracy {
	class Window;

	class OpenGLContext {

	private:

	public:
		OpenGLContext() {}
		~OpenGLContext() = default;

		void Init(void* window_handle);
		void Shutdown() {};

		static shared_ptr<OpenGLContext> Create() {
			return make_shared<OpenGLContext>();
		};
	};
}