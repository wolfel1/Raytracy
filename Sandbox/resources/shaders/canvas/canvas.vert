#version 450 core

const vec2 quad_vertices[4] = { vec2(-1.0, -1.0), vec2(1.0, -1.0), vec2(-1.0, 1.0), vec2(1.0, 1.0) };
const vec2 tex_coords[4] = { vec2(0.0, -0.0), vec2(1.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0) };

layout(location = 0) out vec2 out_vertex_tex_coords;

void main() {
	out_vertex_tex_coords = tex_coords[gl_VertexID];
	gl_Position = vec4(quad_vertices[gl_VertexID], 0.0, 1.0);
}