#include "raytracypch.h"
#include "Raytracer.h"

#include <execution>

namespace raytracy {

	Scene scene{};
	Camera camera{};
	Image image{};

	Raytracer::Raytracer() {}

	Raytracer::~Raytracer() {
		delete[] accumulated_color_data;
	}

	bool HitSphere(const Ray& ray, float min, float max, Hit& hit, Sphere const& sphere) {
		glm::vec3 origin_to_center = ray.origin - sphere.origin;
		auto a = glm::length2(ray.direction);
		auto bias = glm::dot(origin_to_center, ray.direction);
		auto c = glm::length2(origin_to_center) - sphere.radius * sphere.radius;

		auto discriminant = bias * bias - a * c;
		if (discriminant < 0.0f) {
			return false;
		}

		auto sqrt_discriminant = glm::sqrt(discriminant);
		auto hit_value = (-bias - sqrt_discriminant) / a;
		if (hit_value < min || hit_value > max) {
			hit_value = (-bias + sqrt_discriminant) / a;
			if (hit_value < min || hit_value > max) {
				return false;
			}
		}

		hit.hit_value = hit_value;
		hit.point = ray.origin + ray.direction * hit_value;
		hit.normal = glm::normalize(hit.point - sphere.origin);
		hit.color = sphere.color;
		return true;
	}

	bool Trace(const Ray& ray, float min, float max, Hit& hit) {
		Hit temp;
		bool hit_anything = false;
		auto closest = max;

		for (const auto& object : scene.spheres) {
			if (HitSphere(ray, min, closest, temp, object)) {
				closest = temp.hit_value;
				hit_anything = true;
				hit = temp;
			}
		}

		return hit_anything;
	}

	glm::vec4 ComputePixelColor(const Ray& ray) {
		Ray current_ray = ray;
		glm::vec4 current_attenuation(1.0f);
		for (uint32_t i = 0; i < image.max_depth; i++) {
			Hit hit;
			if (Trace(current_ray, 0.001f, infinity, hit)) {
				/*Ray scattered_ray;
				glm::vec4 attenuation(1.0f);
				if (hit.material->Scatter(current_ray, hit, attenuation, scattered_ray)) {
					current_attenuation *= attenuation;
					current_ray = scattered_ray;
				} else {*/
				return hit.color;

			} else {
				glm::vec3 unit_direction = glm::normalize(current_ray.direction);
				auto hit_value = 0.5f * (unit_direction.y + 1.0f);
				glm::vec4 color = glm::vec4(1.0f) * (1.0f - hit_value) +
					glm::vec4(0.5f, 0.7f, 1.0f, 1.0f) * hit_value;
				return current_attenuation * color;
			}
		}
		return glm::vec4(0);
	}

	void WriteImage(const glm::vec4* data) {
		RTY_PROFILE_FUNCTION();
		RTY_ASSERT(data, "No image data!");

		if (data) {

			std::ofstream output_stream("./image.ppm", std::ios::out | std::ios::binary);
			output_stream << "P3\n" << image.width << ' ' << image.height << "\n255\n";
			RTY_RAYTRACER_TRACE("Save as PPM...");

			for (int32_t j = (int32_t)image.height - 1; j >= 0; j--) {
				// left to right
				for (int32_t i = 0; i < (int32_t)image.width; i++) {
					glm::vec4 pixel_color = data[i + j * image.width];

					output_stream << static_cast<int>(255.0f * pixel_color.r) << ' '
						<< static_cast<int>(255.0f * pixel_color.g) << ' '
						<< static_cast<int>(255.0f * pixel_color.b) << '\n';
				}
			}

			output_stream.close();
			RTY_RAYTRACER_TRACE("Saved.");

		}
	}

	void Raytracer::RaytraceScene() {
		const auto aspect_ratio = 1.0f;

		image.width = 720;
		image.height = static_cast<uint32_t>(image.width / aspect_ratio);
		image.samples = 50;
		image.max_depth = 50;


		/*auto world_material =
			make_shared<LambertianDiffuse>(glm::vec4(0.2f, 1.0f, 0.2f, 1.0f));*/
		scene.spheres.push_back({ glm::vec4(0.2f, 1.0f, 0.2f, 1.0f), glm::vec3(0.0f, -101.0f, 0.0f), 100.0f });

		/*auto diffuse_material =
			make_shared<LambertianDiffuse>(glm::vec4(1.0f, 0.2f, 0.2f, 1.0f));*/

		scene.spheres.push_back({ glm::vec4(1.0f, 0.2f, 0.2f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.75f });

		/*auto metal_material =
			make_shared<Metal>(glm::vec4(1.0f, 1.0f, 0.2f, 1.0f), 0.0f);*/

		scene.spheres.push_back({ glm::vec4(1.0f, 1.0f, 0.2f, 1.0f), glm::vec3(2.0f, 0.0f, 0.0f), 0.75f });

		/*auto dielectric_material =
			make_shared<Dielectric>(1.5f);*/

		scene.spheres.push_back({ glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(-2.0f, 0.0f, 0.0f), 0.75f });


		glm::vec3 look_from({ 0.0f, 0.0f, 5.0f });
		glm::vec3 direction({ 0.0f, 0.0f, -1.0f});
		glm::vec3 right{1.0f, 0.0f, 0.0f};
		glm::vec3 up{ 0.0f, 1.0f, 0.0f };

		camera = { look_from, direction, up, right };

		Submit();

		WriteImage(accumulated_color_data);
	}

	void Raytracer::Submit() {
		RTY_ASSERT(image.width * image.height != 0, "Image has no dimensions!");
		accumulated_color_data = new glm::vec4[image.width * image.height];
#define MT 1
#if MT
		horizontal_iterator.resize(image.width);
		vertical_iterator.resize(image.height);
		for (uint32_t i = 0; i < image.width; i++) {
			horizontal_iterator[i] = i;
		}
		for (uint32_t i = 0; i < image.height; i++) {
			vertical_iterator[i] = i;
		}
#endif

		RTY_RAYTRACER_INFO("Successfully submitted!");

		raytracing_thread = std::thread{ &Raytracer::Raytrace, this };
		raytracing_thread.join();
	}

	void Raytracer::Raytrace() {
		RTY_PROFILE_FUNCTION();
		{
			InstrumentationTimer t("Raytracing", true);

			memset(accumulated_color_data, 0, image.width * image.height * sizeof(glm::vec4));

			RTY_RAYTRACER_TRACE("Start process");
#if MT
			std::for_each(std::execution::par, vertical_iterator.begin(), vertical_iterator.end(), [this](uint32_t y) {
				std::cout << "#";
				std::for_each(std::execution::par, horizontal_iterator.begin(), horizontal_iterator.end(), [this, y](uint32_t x) {

					glm::vec4 accumulated_color(0.0f);
					for (uint32_t sample = 0; sample < image.samples; ++sample) {
						glm::vec2 coord = { (float)x / (float)image.width, (float)y / (float)image.height };
						coord = coord * 2.0f - 1.0f;
						Ray ray{};
						ray.origin = camera.position;
						ray.direction = glm::normalize(camera.direction + coord.s * camera.right + coord.t * camera.up);
						accumulated_color += ComputePixelColor(ray);
					}
					accumulated_color /= (float)image.samples;
					accumulated_color = glm::sqrt(accumulated_color);
					accumulated_color = glm::clamp(accumulated_color, glm::vec4(0.0f), glm::vec4(1.0f));
					accumulated_color_data[x + y * image.width] = accumulated_color;
					});
				});
			std::cout << "\n";
#else
			// down to top
			for (uint32_t y = 0; y < image_ptr->GetHeight(); y++) {
				RTY_PROFILE_SCOPE("Line");
				RTY_RAYTRACER_TRACE("Scanlines remaining: {0}", image_ptr->GetHeight() - y);
				// left to right
				for (uint32_t x = 0; x < image_ptr->GetWidth(); x++) {
					RTY_PROFILE_SCOPE("Pixel");
					glm::vec4 accumulated_color(0.0f);
					for (uint32_t sample = 0; sample < image_ptr->GetSamplesPerPixel(); ++sample) {
						auto u = (float(x) + Random::RandomFloat()) / (image_ptr->GetWidth() - 1);
						auto v = (float(y) + Random::RandomFloat()) / (image_ptr->GetHeight() - 1);
						Ray ray = active_camera->ShootRay(u, v);
						accumulated_color += ComputePixelColor(ray, *active_scene, image_ptr->GetMaxDepth());
					}
					accumulated_color /= (float)image_ptr->GetSamplesPerPixel();
					accumulated_color = glm::sqrt(accumulated_color);
					accumulated_color = glm::clamp(accumulated_color, glm::vec4(0.0f), glm::vec4(1.0f));
					accumulated_color_data[x + y * image_ptr->GetWidth()] = accumulated_color;
				}
			}
#endif
			RTY_RAYTRACER_TRACE("Done.");
		}
	}







}  // namespace raytracy
