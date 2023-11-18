#version 450 core

layout(location = 0) in vec4 vertex_color;

layout(std140, binding = 0) uniform Shading {
  vec4 display_color;
  vec3 light_color;
};

layout(location = 0) out vec4 color;

void main() {
	float ambient_strength = 0.1;
	vec3 ambient = ambient_strength * light_color;
	color = vec4(ambient, 1.0) * display_color;
}