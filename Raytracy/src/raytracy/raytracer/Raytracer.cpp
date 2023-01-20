#include "raytracypch.h"
#include "Raytracer.h"

#include <execution>

namespace raytracy {

	Raytracer::Raytracer() {}

	Raytracer::~Raytracer() {
		raytracing_thread.join();
	}

	void Raytracer::Submit(const Scene& scene, const Camera& camera,
		const Image& image_data) {
		active_scene = std::make_unique<Scene>(scene);
		active_camera = std::make_unique<Camera>(camera);
		image = std::make_shared<Image>(image_data);

		raytracing_thread = std::thread{&Raytracer::RayTrace, this};
	}

	void Raytracer::RayTrace() {
		RTY_PROFILE_FUNCTION();
		InstrumentationTimer t("Raytracing", true);

		std::ofstream output_stream("./image.ppm", std::ios::out | std::ios::binary);
		output_stream << "P3\n" << image->width << ' ' << image->height << "\n255\n";
		RTY_RAYTRACER_TRACE("Start process");

		// top to down
		for (int32_t j = image->height - 1; j >= 0; --j) {
			RTY_PROFILE_SCOPE("Line")
				RTY_RAYTRACER_TRACE("Scanlines remaining: {0}", j);
			// left to right
			for (int32_t i = 0; i < image->width; ++i) {
				RTY_PROFILE_SCOPE("Pixel")
					glm::vec3 pixel_color(0);
				for (uint32_t sample = 0; sample < image->samples_per_pixel; ++sample) {
					auto u = (float(i) + Random::RandomFloat()) / (image->width - 1);
					auto v = (float(j) + Random::RandomFloat()) / (image->height - 1);
					Ray ray = active_camera->ShootRay(u, v);
					pixel_color += ComputePixelColor(ray, *active_scene, image->max_depth);
				}

				WriteColor(output_stream, pixel_color, image->samples_per_pixel);
			}
		}
		output_stream.close();

		RTY_RAYTRACER_TRACE("Done.");
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

	void Raytracer::WriteColor(std::ofstream& out, glm::vec3 pixel_color,
		uint32_t samples_per_pixel) {
		auto scale = 1.0f / samples_per_pixel;
		pixel_color = glm::sqrt(scale * pixel_color);
		pixel_color = glm::clamp(pixel_color, 0.0f, 1.0f);

		out << static_cast<int>(255.0f * pixel_color.r) << ' '
			<< static_cast<int>(255.0f * pixel_color.g) << ' '
			<< static_cast<int>(255.0f * pixel_color.b) << '\n';
	}

}  // namespace raytracy
