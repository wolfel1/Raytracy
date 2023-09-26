#version 450 core

layout(location = 0) in vec3 in_vertex_position;
layout(location = 1) in vec4 in_vertex_color;

layout(binding = 0) uniform UniformBufferColorObject {
  vec4 color;
} ubo_color;

layout(binding = 1) uniform UniformBufferCameraObject {
  mat4 model;
  mat4 view;
  mat4 projection;
} ubo_camera;

layout(location = 0) out vec4 display_color;

void main() {
	display_color = ubo_color.color;
	gl_Position = ubo_camera.projection * ubo_camera.view * ubo_camera.model * vec4(in_vertex_position, 1.0);
}