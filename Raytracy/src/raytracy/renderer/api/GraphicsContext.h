#pragma once

namespace raytracy {
	class GraphicsContext {

	protected:
		void* window_handle;

	public:
		static unique_ptr<GraphicsContext> Create(void* window_handle);

		virtual void Init() = 0;

	};
}