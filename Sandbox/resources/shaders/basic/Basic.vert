#version 450 core

layout(location = 0) in vec3 in_vertex_position;
layout(location = 1) in vec3 in_vertex_normal;
layout(location = 2) in vec4 in_vertex_color;

layout(std140, binding = 0) uniform Camera {
  mat4 model;
  mat4 view;
  mat4 projection;
};

layout(location = 0) out vec3 out_vertex_normal;
layout(location = 1) out vec4 out_vertex_color;
layout(location = 2) out vec3 out_frag_position;

void main() {
	out_vertex_normal = in_vertex_normal;
	out_frag_position = vec3(model * vec4(in_vertex_position, 1.0));
	out_vertex_color = in_vertex_color;
	gl_Position = projection * view * model * vec4(in_vertex_position, 1.0);
}