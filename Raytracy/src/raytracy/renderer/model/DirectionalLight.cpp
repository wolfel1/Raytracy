#include "raytracypch.h"
#include "DirectionalLight.h"

namespace raytracy {

	DirectionalLight::DirectionalLight(glm::vec3 color, glm::vec3 direction, float strength)
		: color(color), direction(direction), strength(strength) {
	}

	void DirectionalLight::SetUniformBuffer(shared_ptr<OpenGLUniformBuffer> buffer) {
		buffer->SetVec3("lightColor", color);
		buffer->SetVec3("direction", direction);
		buffer->SetFloat("strength", strength);

		light_uniform_buffer = buffer;
	}
}