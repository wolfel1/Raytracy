#pragma once


namespace raytracy {
	class Window;

	class GraphicsContext {

	protected:
		shared_ptr<Window> window;
		static shared_ptr<GraphicsContext> current_context;

	public:
		virtual ~GraphicsContext() = default;

		static shared_ptr<GraphicsContext> Create();
		static shared_ptr<GraphicsContext> GetCurrentContext()  { return current_context; }

		virtual void SetWindow(const shared_ptr<Window>& window) { this->window = window; }
		virtual void Init(void* window_handle) = 0;
		virtual void SwapBuffers() = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual void Shutdown() = 0;


	};
}