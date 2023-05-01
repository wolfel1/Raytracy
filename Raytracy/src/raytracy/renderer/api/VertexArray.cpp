#include "raytracypch.h"
#include "VertexArray.h"

#include "opengl/OpenGLVertexArray.h"

namespace raytracy {
	shared_ptr<VertexArray> VertexArray::Create() {
		return make_shared<OpenGLVertexArray>();
	}
}