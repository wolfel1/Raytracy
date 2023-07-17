#pragma once

#include "../GraphicsContext.h"

namespace raytracy {
	class OpenGLContext : public GraphicsContext {

	public:
		OpenGLContext() = default;

		virtual void Init(void* window_handle) override;
		virtual void SwapBuffers() override;
		virtual void SetVSync(bool enabled) override;
		virtual void Shutdown() override {};
	};
}