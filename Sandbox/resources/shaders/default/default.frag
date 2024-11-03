#version 450 core

layout(location = 0) in vec3 vertex_normal;
layout(location = 1) in vec4 vertex_color;
layout(location = 2) in vec2 vertex_tex_coords;
layout(location = 3) in vec3 frag_position;
layout(location = 4) in vec3 camera_position;


layout(std140, binding = 1) uniform Material {
  vec4 color;
};

layout(std140, binding = 2) uniform DirectionalLight {
  vec3 lightColor;
  vec3 direction;
  float strength;
};

layout(location = 0) out vec4 out_color;

uniform samplerCube skybox;

void main() {
	vec3 normal = normalize(vertex_normal);
	vec3 incoming = normalize(frag_position - camera_position);
	vec3 reflection = reflect(incoming, normal);

	float facingLight = max(dot(normal, -direction), 0.0);

	vec3 result = texture(skybox, reflection).rgb * color.rgb * facingLight;
	out_color = vec4(result, color.a);
}