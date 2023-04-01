#version 450 core

in vec4 vertex_color;

layout(location = 0) out vec4 color;

void main() {
	color = vertex_color;
}