#pragma once

#include "../api/opengl/OpenGLBuffer.h"

namespace raytracy {

	class DirectionalLight {
	private:
		glm::vec3 color;
		glm::vec3 direction;
		float strength;

		shared_ptr<OpenGLUniformBuffer> light_uniform_buffer;

	};
}