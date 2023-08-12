#version 450 core

layout(location = 0) in vec3 in_vertex_position;
layout(location = 1) in vec4 in_vertex_color;

layout(binding = 0) uniform UniformBufferColorObject {
  vec4 color;
} ubo_color;

layout(location = 0) out vec4 vertex_color;

void main() {
	vertex_color = ubo_color.color;
	gl_Position = vec4(in_vertex_position, 1.0);
}