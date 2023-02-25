#type vertex
#version 450 core

layout(location = 0) in vec3 aPosition;

void main() {
	gl_Position = vec4(aPosition, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;

void main() {
	color = vec4(0.8f, 0.2f, 0.3f, 1.0f);
}