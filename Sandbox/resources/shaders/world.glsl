#type vertex
#version 450 core

layout(location = 0) in vec3 in_vertex_position;
layout(location = 1) in vec3 in_vertex_normal;
layout(location = 2) in vec4 in_vertex_color;
layout(location = 3) in vec2 in_vertex_tex_coords;

layout(std140, binding = 0) uniform Camera {
  mat4 model_view_matrix;
  mat4 view_projection_matrix;
  mat4 model_view_projection_matrix;
  mat4 normal_matrix;
};

layout(location = 0) out vec3 out_tex_coords;

void main() {
	out_tex_coords = in_vertex_position;
	vec4 position = view_projection_matrix * vec4(in_vertex_position, 1.0);
	gl_Position = position.xyww;
}

#type fragment
#version 450 core

layout(location = 0) in vec3 tex_coords;

layout(location = 0) out vec4 out_color;

void main() {
	out_color = vec4(tex_coords, 1.0);
}