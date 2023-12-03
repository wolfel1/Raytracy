#version 450 core

layout(location = 0) in vec3 vertex_normal;
layout(location = 1) in vec4 vertex_color;
layout(location = 2) in vec3 frag_position;

layout(std140, binding = 1) uniform Shading {
  vec4 display_color;
  vec3 light_color;
  vec3 light_position;
};

layout(location = 0) out vec4 color;

void main() {
	float ambient_strength = 0.1;
	vec3 ambient = ambient_strength * light_color;

	vec3  normal = normalize(vertex_normal);
	vec3 light_direction = normalize(light_position - frag_position);
	float diffuse_impact = max(0.0, dot(normal, light_direction));
	vec3 diffuse = diffuse_impact * light_color;

	vec3 result = (ambient + diffuse) * display_color.rgb;
	color = vec4(result, display_color.a);
}