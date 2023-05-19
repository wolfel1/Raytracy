#pragma once

#include "../GraphicsContext.h"

namespace raytracy {
	class OpenGLContext : public GraphicsContext {

	public:
		OpenGLContext(void* window_handle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void Shutdown() override {};
	};
}