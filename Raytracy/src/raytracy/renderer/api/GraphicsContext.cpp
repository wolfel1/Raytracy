#include "raytracypch.h"

#include "GraphicsContext.h"


namespace raytracy {
	unique_ptr<GraphicsContext> GraphicsContext::Create(void* window_handle) {
		return unique_ptr<GraphicsContext>();
	}
}