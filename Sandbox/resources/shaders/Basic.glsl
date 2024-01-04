#type vertex
#version 450 core

layout(location = 0) in vec3 in_vertex_position;
layout(location = 1) in vec3 in_vertex_normal;
layout(location = 2) in vec4 in_vertex_color;

layout(std140, binding = 0) uniform Camera {
  mat4 model_view_matrix;
  mat4 model_view_projection_matrix;
  mat4 normal_matrix;
};

layout(location = 0) out vec3 out_vertex_normal;
layout(location = 1) out vec4 out_vertex_color;
layout(location = 2) out vec3 out_frag_position;

void main() {
	out_vertex_normal = mat3(normal_matrix) * in_vertex_normal;
	out_frag_position = vec3(model_view_matrix * vec4(in_vertex_position, 1.0));
	out_vertex_color = in_vertex_color;
	gl_Position = model_view_projection_matrix * vec4(in_vertex_position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) in vec3 vertex_normal;
layout(location = 1) in vec4 vertex_color;
layout(location = 2) in vec3 frag_position;

layout(std140, binding = 1) uniform Light {
  vec3 light_color;
  vec3 light_direction;
  float light_strength;
};

layout(std140, binding = 2) uniform Material {
  vec4 color;
};

layout(location = 0) out vec4 out_color;

void main() {
	vec3 light_ambient = light_color * light_strength;

	vec3  normal = normalize(vertex_normal);
	float diffuse_impact = max(0.0, dot(normal, light_direction));
	vec3 diffuse = diffuse_impact * light_color;

	vec3 result = (light_ambient + diffuse) * color.rgb;
	out_color = vec4(result, color.a);
}