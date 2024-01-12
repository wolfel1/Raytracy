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

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform image2D imgOutput;

layout(std430, binding = 0) buffer Scene {
    Sphere spheres[];
};

vec4 computePixelColor(Ray ray);
bool trace(inout Ray ray, float minimum, float maximum, inout Hit hit);
bool hitSphere(const Ray ray, float minimum, float maximum, inout Hit hit, const Sphere sphere);

void main() {
    vec4 value = vec4(0.0, 0.0, 0.0, 1.0);
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
    ivec2 image_size = imageSize(imgOutput);

    if (texelCoord.x >= image_size.x || texelCoord.y >= image_size.y) {
        return;
    }

    float horizontal_coefficient = (float(texelCoord.x) - float(image_size.x) / 2) / float(image_size.x);
    float vertical_coefficient = (float(texelCoord.y) - float(image_size.y) / 2) / float(image_size.x);
    vec3 direction = vec3(0.0, 0.0, -1.0);
    vec3 right = vec3(1.0, 0.0, 0.0);
    vec3 up = vec3(0.0, 1.0, 0.0);

    vec4 accumulated_color = vec4(0.0);
    for (int test = 0; test < 10; ++test) {
        Ray ray;
        ray.origin = vec3(0.0, 0.0, 5.0);
        ray.direction = normalize(direction + horizontal_coefficient * right + vertical_coefficient * up);
        accumulated_color += computePixelColor(ray);
    }
    accumulated_color /= float(10);
    accumulated_color = sqrt(accumulated_color);
    accumulated_color = clamp(accumulated_color, vec4(0.0), vec4(1.0));
        
    imageStore(imgOutput, texelCoord, accumulated_color);
}

vec4 computePixelColor(Ray ray) {
    Ray current_ray = ray;
    vec4 current_attenuation = vec4(1.0);
    for (int i = 0; i < 10; i++) {
        Hit hit;
        if (trace(current_ray, 0.001, 3.40282e+038, hit)) {
            // auto scatter_direction = hit.normal + Random::RandomUnitVector();

            // if (glm::all(glm::lessThan(glm::abs(scatter_direction), glm::vec3(1e-8f)))) {
            //     scatter_direction = hit.normal;
            // }

            // current_ray = Ray(hit.point, hit.normal);
            // current_attenuation *= hit.color;
            return hit.color;
        } else {
            vec3 unit_direction = normalize(current_ray.direction);
            float hit_value = 0.5 * (unit_direction.y + 1.0);
            vec4 color = vec4(0.1, 0.8, 0.8, 1.0);
            return current_attenuation * color;
        }
    }
    return vec4(0);
}

bool trace(inout Ray ray, float minimum, float maximum, inout Hit hit) {
	Hit temp;
	bool hit_anything = false;
	float closest = maximum;

	for (int i = 0; i < spheres.length(); i++) {
		if (hitSphere(ray, minimum, closest, temp, spheres[i])) {
			closest = temp.hit_value;
			hit_anything = true;
			hit = temp;
		}
	}

	return hit_anything;
}

bool hitSphere(const Ray ray, float minimum, float maximum, inout Hit hit, const Sphere sphere) {
	vec3 origin_to_center = ray.origin - sphere.origin;
	float a = length(ray.direction) * length(ray.direction);
	float bias = dot(origin_to_center, ray.direction);
	float c = length(origin_to_center) * length(origin_to_center) - sphere.radius * sphere.radius;

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
	hit.normal = (hit.point - sphere.origin) / sphere.radius;
	//hit.SetFaceNormal(ray, outward_normal);
	hit.color = sphere.color;
	return true;
}