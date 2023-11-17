#version 450 core

layout(location = 0) in vec4 vertex_color;

layout(std140, binding = 0) uniform Shading {
  vec4 display_color;
};

layout(location = 0) out vec4 color;

void main() {
	color = display_color;
}