#version 450 core

layout(location = 0) in vec3 in_vertex_position;
layout(location = 1) in vec4 in_vertex_color;

layout(std140, binding = 0) uniform Shading {
  vec4 color;
};

layout(std140, binding = 1) uniform Camera {
  mat4 model;
  mat4 view;
  mat4 projection;
};

layout(location = 0) out vec4 display_color;

void main() {
	display_color = color;
	gl_Position = projection * view * model * vec4(in_vertex_position, 1.0f);
}