#version 450 core

layout(location = 0) in vec3 vertex_normal;
layout(location = 1) in vec4 vertex_color;
layout(location = 2) in vec2 vertex_tex_coords;
layout(location = 3) in vec3 frag_position;
layout(location = 4) in vec3 camera_position;


layout(std140, binding = 1) uniform Material {
  vec4 color;
  vec3 specular;
  float shininess;
};

layout(std140, binding = 2) uniform DirectionalLight {
  vec3 lightColor;
  vec3 direction;
  float strength;
};

layout(location = 0) out vec4 out_color;

uniform samplerCube skybox;

void main() {
	vec3 normal = normalize(vertex_normal);
	vec3 viewDirection = normalize(camera_position - frag_position);
	vec3 lightDirection = normalize(-direction);

	vec3 ambient = 0.1 * color.rgb;

	float diff = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = diff * lightColor * color.rgb;

	vec3 halfwayDir = normalize(lightDirection + viewDirection);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
	vec3 specular = spec * specular * lightColor;

	vec3 result = (ambient + diffuse + specular) * strength;
	out_color = vec4(result, color.a);
}