#pragma once

namespace raytracy {
	class GraphicsContext {

	protected:
		void* window_handle;

	public:
		virtual ~GraphicsContext() = default;

		static unique_ptr<GraphicsContext> Create(void* window_handle);

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

	protected:
		GraphicsContext(void* window_handle);
	};
}