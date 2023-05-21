#pragma once


namespace raytracy {
	class Window;

	class GraphicsContext {

	protected:
		shared_ptr<Window> window;

	public:
		virtual ~GraphicsContext() = default;

		static shared_ptr<GraphicsContext> Create(const shared_ptr<Window>& window_handle);

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual void Shutdown() = 0;

	protected:
		GraphicsContext(const shared_ptr<Window> window);

	};
}