#version 450 core

layout(location = 0) in vec2 tex_coords;

layout(location = 0) out vec4 out_color;

uniform sampler2D tex;

void main() {
	vec3 color = texture(tex, tex_coords).rgb;
	out_color = vec4(color, 1.0);
}