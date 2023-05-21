#pragma once

#include "../GraphicsContext.h"

namespace raytracy {
	class OpenGLContext : public GraphicsContext {

	public:
		OpenGLContext(const shared_ptr<Window>& window);

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void SetVSync(bool enabled) override;
		virtual void Shutdown() override {};
	};
}