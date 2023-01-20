#include "raytracypch.h"
#include "Raytracer.h"

#include <execution>

namespace raytracy {

	Raytracer::Raytracer() {}

	Raytracer::~Raytracer() {
		raytracing_thread.join();

		delete[] accumalated_color_data;
	}

	void Raytracer::Submit(const Scene& scene, const Camera& camera,
		const Image& image_data) {
		active_scene = std::make_unique<Scene>(scene);
		active_camera = std::make_unique<Camera>(camera);
		image = std::make_shared<Image>(image_data);
		accumalated_color_data = new glm::vec3[image->width * image->height];

		raytracing_thread = std::thread{ &Raytracer::RayTrace, this };
	}

	void Raytracer::RayTrace() {
		RTY_PROFILE_FUNCTION();
		{
			InstrumentationTimer t("Raytracing", true);

			memset(accumalated_color_data, 0, image->width * image->height * sizeof(glm::vec3));

			RTY_RAYTRACER_TRACE("Start process");

			// down to top
			for (uint32_t y = 0; y < image->height; y++) {
				RTY_PROFILE_SCOPE("Line");
				RTY_RAYTRACER_TRACE("Scanlines remaining: {0}", image->height - y);
				// left to right
				for (uint32_t x = 0; x < image->width; x++) {
					RTY_PROFILE_SCOPE("Pixel");
					accumalated_color_data[x + y * image->width] = glm::vec3(0);
					for (uint32_t sample = 0; sample < image->samples_per_pixel; ++sample) {
						auto u = (float(x) + Random::RandomFloat()) / (image->width - 1);
						auto v = (float(y) + Random::RandomFloat()) / (image->height - 1);
						Ray ray = active_camera->ShootRay(u, v);
						accumalated_color_data[x + y * image->width] += ComputePixelColor(ray, *active_scene, image->max_depth);
					}
				}
			}

			RTY_RAYTRACER_TRACE("Done.");
		}

		WriteImage();
	}

	glm::vec3 Raytracer::ComputePixelColor(const Ray& ray,
		const Scene& objects,
		uint32_t depth) {
		Hit hit;
		if (depth <= 0) {
			return glm::vec3(0.0f);
		}

		auto hit_anything = objects.HitObjects(ray, 0.001f, infinity, hit);

		if (hit_anything) {
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
	}

	void Raytracer::WriteImage() {
		RTY_PROFILE_FUNCTION();
		InstrumentationTimer t("Save", true);

		std::ofstream output_stream("./image.ppm", std::ios::out | std::ios::binary);
		output_stream << "P3\n" << image->width << ' ' << image->height << "\n255\n";
		RTY_RAYTRACER_TRACE("Save as PPM...");

		for (int32_t j = (int32_t) image->height - 1; j >= 0; j--) {
			// left to right
			for (int32_t i = 0; i < (int32_t) image->width; i++) {
				glm::vec3 pixel_color = accumalated_color_data[i + j * image->width];

				auto scale = 1.0f / image->samples_per_pixel;
				pixel_color = glm::sqrt(scale * pixel_color);
				pixel_color = glm::clamp(pixel_color, 0.0f, 1.0f);

				output_stream << static_cast<int>(255.0f * pixel_color.r) << ' '
					<< static_cast<int>(255.0f * pixel_color.g) << ' '
					<< static_cast<int>(255.0f * pixel_color.b) << '\n';
			}
		}

		output_stream.close();
		RTY_RAYTRACER_TRACE("Done.");
	}

}  // namespace raytracy
