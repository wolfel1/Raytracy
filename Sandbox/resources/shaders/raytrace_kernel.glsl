#type compute
#version 450 core

struct Sphere {
	vec4 color;
	vec3 origin;
	float radius;
};

struct Ray {
	vec3 origin;
	vec3 direction;
};

struct Hit {
	vec4 color;
	float hit_value;
	vec3 point;
	vec3 normal;
};

struct Camera {
	vec3 position;
	vec3 direction;
	vec3 up;
	vec3 right;
};

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform image2D imgOutput;

layout(std140, binding = 0) uniform SceneData {
    mat4 inverse_view;
    mat4 inverse_projection;
    vec3 camera_position;
	int samples;
	vec3 camera_direction;
	int max_depth;
};

layout(std430, binding = 0) buffer Scene {
    Sphere spheres[];
};

uniform samplerCube skybox;

vec4 computePixelColor(Ray ray);
bool trace(in Ray ray, float minimum, float maximum, inout Hit hit);
bool hitSphere(in Ray ray, float minimum, float maximum, inout Hit hit, const Sphere sphere);


void main() {
    vec4 value = vec4(0.0, 0.0, 0.0, 1.0);
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
    ivec2 image_size = imageSize(imgOutput);

    if (texelCoord.x >= image_size.x || texelCoord.y >= image_size.y) {
        return;
    }

    vec3 direction = normalize(camera_direction);
    vec2 coord = vec2(float(texelCoord.x) / float(image_size.x), float(texelCoord.y) / float(image_size.y));
    coord = coord * 2.0 - 1.0; // -1 -> 1
    vec4 target = inverse_projection * vec4(coord.x, coord.y, 1, 1);
    vec3 ray_direction = vec3(inverse_view * vec4(normalize(vec3(target) / target.w), 0));

    vec4 accumulated_color = vec4(0.0);
    for (int test = 0; test < samples; ++test) {
        Ray ray;
        ray.origin = camera_position;

        ray.direction = ray_direction;
        accumulated_color += computePixelColor(ray);
    }
    accumulated_color /= float(samples);
    accumulated_color = clamp(accumulated_color, vec4(0.0), vec4(1.0));
        
    imageStore(imgOutput, texelCoord, accumulated_color);
}

vec4 computePixelColor(Ray ray) {
    Ray current_ray = ray;
    vec4 current_attenuation = vec4(1.0);
    for (int i = 0; i < max_depth; i++) {
        Hit hit;
        if (trace(current_ray, 0.001, 3.40282e+038, hit)) {
            vec3 scatter_direction = reflect(current_ray.direction, hit.normal);

            current_ray.origin = hit.point;
            current_ray.direction = scatter_direction;
            current_attenuation = current_attenuation * hit.color;
        } else {
            vec3 unit_direction = normalize(current_ray.direction);
			float ambient = 0.5 * (unit_direction.y + 1.0);
            vec4 color = texture(skybox, unit_direction);
            return current_attenuation * color;
        }
    }
    return vec4(0);
}

bool trace(in Ray ray, float minimum, float maximum, inout Hit hit) {
	Hit temp;
	bool hit_anything = false;
	float closest = maximum;

	for (int i = 0; i < spheres.length(); ++i) {
		if (hitSphere(ray, minimum, closest, temp, spheres[i])) {
			closest = temp.hit_value;
			hit_anything = true;
			hit = temp;
		}
	}

	return hit_anything;
}

bool hitSphere(in Ray ray, float minimum, float maximum, inout Hit hit, const Sphere sphere) {
	vec3 origin_to_center = ray.origin - sphere.origin;
	float a = dot(ray.direction, ray.direction);
	float bias = dot(origin_to_center, ray.direction);
	float c = dot(origin_to_center, origin_to_center) - sphere.radius * sphere.radius;

	float discriminant = bias * bias - a * c;
	if (discriminant < 0.0) {
		return false;
	}

	float sqrt_discriminant = sqrt(discriminant);
	float hit_value = (-bias - sqrt_discriminant) / a;
	if (hit_value < minimum || hit_value > maximum) {
		hit_value = (-bias + sqrt_discriminant) / a;
		if (hit_value < minimum || hit_value > maximum) {
			return false;
		}
	}

	hit.hit_value = hit_value;
	hit.point = ray.origin + ray.direction * hit_value;
	hit.normal = (hit.point - sphere.origin ) / sphere.radius;
	//hit.SetFaceNormal(ray, outward_normal);
	hit.color = sphere.color;
	return true;
}