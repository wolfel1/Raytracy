#include "raytracypch.h"
#include "VertexArray.h"

#include "vulkan/VulkanVertexArray.h"

namespace raytracy {
	shared_ptr<VertexArray> VertexArray::Create() {
		return make_shared<VulkanVertexArray>();
	}
}