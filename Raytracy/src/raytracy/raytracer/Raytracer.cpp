#include "raytracypch.h"
#include "Raytracer.h"

#include <execution>

namespace raytracy {

	Raytracer::Raytracer() {}

	Raytracer::~Raytracer() {

		raytracing_thread.join();
		delete[] accumulated_color_data;
	}

	void Raytracer::Submit(const Scene& scene, const Camera& camera,
		const Image& image_data) {
		active_scene = std::make_unique<Scene>(scene);
		active_camera = std::make_unique<Camera>(camera);
		image = std::make_shared<Image>(image_data);
		accumulated_color_data = new glm::vec3[image->width * image->height];
#define MT 1
#if MT
		horizontal_iterator.resize(image->width);
		vertical_iterator.resize(image->height);
		for (uint32_t i = 0; i < image->width; i++) {
			horizontal_iterator[i] = i;
		}
		for (uint32_t i = 0; i < image->height; i++) {
			vertical_iterator[i] = i;
		}
#endif

		raytracing_thread = std::thread{ &Raytracer::RayTrace, this };
	}

	void Raytracer::RayTrace() {
		RTY_PROFILE_FUNCTION();
		{
			InstrumentationTimer t("Raytracing", true);

			memset(accumulated_color_data, 0, image->width * image->height * sizeof(glm::vec3));

			RTY_RAYTRACER_TRACE("Start process");
#if MT

			std::for_each(std::execution::par, vertical_iterator.begin(), vertical_iterator.end(), [this](uint32_t y) {
				std::for_each(std::execution::par, horizontal_iterator.begin(), horizontal_iterator.end(), [this, y](uint32_t x) {

					glm::vec3 accumulated_color(0);
					for (uint32_t sample = 0; sample < image->samples_per_pixel; ++sample) {
						auto u = (float(x) + Random::RandomFloat()) / (image->width - 1);
						auto v = (float(y) + Random::RandomFloat()) / (image->height - 1);
						Ray ray = active_camera->ShootRay(u, v);
						accumulated_color += ComputePixelColor(ray, *active_scene, image->max_depth);
					}
					accumulated_color /= image->samples_per_pixel;
					accumulated_color = glm::sqrt(accumulated_color);
					accumulated_color = glm::clamp(accumulated_color, glm::vec3(0.0f), glm::vec3(1.0f));
					accumulated_color_data[x + y * image->width] = accumulated_color;
					});
				});
#else
			// down to top
			for (uint32_t y = 0; y < image->height; y++) {
				RTY_PROFILE_SCOPE("Line");
				RTY_RAYTRACER_TRACE("Scanlines remaining: {0}", image->height - y);
				// left to right
				for (uint32_t x = 0; x < image->width; x++) {
					RTY_PROFILE_SCOPE("Pixel");
					glm::vec3 accumulated_color(0);
					for (uint32_t sample = 0; sample < image->samples_per_pixel; ++sample) {
						auto u = (float(x) + Random::RandomFloat()) / (image->width - 1);
						auto v = (float(y) + Random::RandomFloat()) / (image->height - 1);
						Ray ray = active_camera->ShootRay(u, v);
						accumulated_color += ComputePixelColor(ray, *active_scene, image->max_depth);
					}
					accumulated_color /= image->samples_per_pixel;
					accumulated_color = glm::sqrt(accumulated_color);
					accumulated_color = glm::clamp(accumulated_color, glm::vec3(0.0f), glm::vec3(1.0f));
					accumulated_color_data[x + y * image->width] = accumulated_color;
				}
			}
#endif

			RTY_RAYTRACER_TRACE("Done.");
		}

		WriteImage();
	}

	glm::vec3 Raytracer::ComputePixelColor(const Ray& ray,
		const Scene& objects,
		uint32_t depth) {
#define RECURSIVE 1
#if RECURSIVE
		Hit hit;
		if (depth <= 0) {
			return glm::vec3(0.0f);
		}

		if (objects.HitObjects(ray, 0.001f, infinity, hit)) {
			Ray scattered_ray;
			glm::vec3 attenuation{};
			if (hit.material->Scatter(ray, hit, attenuation, scattered_ray)) {
				return ComputePixelColor(scattered_ray, objects, depth - 1) * attenuation;
			}
			return glm::vec3(0.0f);
		}

		glm::vec3 unit_direction = glm::normalize(ray.GetDirection());
		auto hit_value = 0.5f * (unit_direction.y + 1.0f);
		return glm::vec3(1.0f) * (1.0f - hit_value) +
			glm::vec3(0.5f, 0.7f, 1.0f) * hit_value;
#else
		Ray current_ray = ray;
		glm::vec3 current_attenuation(1.0f);
		for (uint32_t i = 0; i < depth; i++) {
			Hit hit;
			if (objects.HitObjects(current_ray, 0.001f, infinity, hit)) {
				Ray scattered_ray;
				glm::vec3 attenuation(0);
				if (hit.material->Scatter(current_ray, hit, attenuation, scattered_ray)) {
					current_attenuation *= attenuation;
					current_ray = scattered_ray;
				} else {
					return glm::vec3(0);
				}
			} else {
				glm::vec3 unit_direction = glm::normalize(current_ray.GetDirection());
				auto hit_value = 0.5f * (unit_direction.y + 1.0f);
				glm::vec3 color = glm::vec3(1.0f) * (1.0f - hit_value) +
					glm::vec3(0.5f, 0.7f, 1.0f) * hit_value;
				return current_attenuation * color;
			}
		}
		return glm::vec3(0);
#endif
	}

	void Raytracer::WriteImage() {
		RTY_PROFILE_FUNCTION();
		InstrumentationTimer t("Save", true);

		std::ofstream output_stream("./image.ppm", std::ios::out | std::ios::binary);
		output_stream << "P3\n" << image->width << ' ' << image->height << "\n255\n";
		RTY_RAYTRACER_TRACE("Save as PPM...");

		for (int32_t j = (int32_t)image->height - 1; j >= 0; j--) {
			// left to right
			for (int32_t i = 0; i < (int32_t)image->width; i++) {
				glm::vec3 pixel_color = accumulated_color_data[i + j * image->width];

				output_stream << static_cast<int>(255.0f * pixel_color.r) << ' '
					<< static_cast<int>(255.0f * pixel_color.g) << ' '
					<< static_cast<int>(255.0f * pixel_color.b) << '\n';
			}
		}

		output_stream.close();
		RTY_RAYTRACER_TRACE("Done.");
	}

}  // namespace raytracy
