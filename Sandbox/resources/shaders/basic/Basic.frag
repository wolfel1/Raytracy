#version 450 core

layout(location = 0) in vec3 vertex_normal;
layout(location = 1) in vec4 vertex_color;
layout(location = 2) in vec3 frag_position;



layout(std140, binding = 1) uniform Light {
  vec3 light_color;
  vec3 light_direction;
  float light_strength;
};
layout(std140, binding = 2) uniform Shading {
  vec4 display_color;
};


layout(location = 0) out vec4 out_color;

void main() {
	float ambient_strength = 1.0;
	vec3 ambient = ambient_strength * light_color * light_strength;

	vec3  normal = normalize(vertex_normal);
	float diffuse_impact = max(0.0, dot(normal, light_direction));
	vec3 diffuse = diffuse_impact * light_color;

	vec3 result = (ambient + diffuse) * display_color.rgb;
	out_color = vec4(result, display_color.a);
}