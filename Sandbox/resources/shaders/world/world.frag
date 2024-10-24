#version 450 core

layout(location = 0) in vec3 tex_coords;

layout(location = 0) out vec4 out_color;

uniform samplerCube skybox;

void main() {
	out_color = texture(skybox, tex_coords);
}