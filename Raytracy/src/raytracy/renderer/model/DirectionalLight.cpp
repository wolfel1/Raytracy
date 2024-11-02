#include "raytracypch.h"
#include "DirectionalLight.h"

namespace raytracy {

	shared_ptr<OpenGLUniformBuffer> DirectionalLight::light_uniform_buffer = nullptr;

	DirectionalLight::DirectionalLight(glm::vec3 color, glm::vec3 direction, float strength)
		: color(color), direction(direction), strength(strength) {
	}

}