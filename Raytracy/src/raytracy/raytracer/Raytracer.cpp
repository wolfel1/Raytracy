#include "raytracypch.h"
#include "Raytracer.h"

#include <thread>
#include <execution>

namespace raytracy {

	static std::mutex output_mutex;
	static std::mutex hit_mutex;

	void Raytracer::Init() {}

	void Raytracer::Submit(const Scene& objects, const Camera& camera,
		const ImageData& data) {
		RTY_PROFILE_FUNCTION();
		InstrumentationTimer t("Raytracing", true);

		std::ofstream output_stream("./image.ppm", std::ios::out | std::ios::binary);
		output_stream << "P3\n" << data.width << ' ' << data.height << "\n255\n";
		RTY_RAYTRACER_TRACE("Start process");

#define MT 0
#if MT
		std::vector<int32_t> image_horizontal_iterator, image_vertical_iterator;
		for (int32_t i = 0; i < data.width; ++i) { image_horizontal_iterator.push_back(i); }
		for (int32_t i = data.height - 1; i >= 0; --i) { image_vertical_iterator.push_back(i); }
		std::for_each(
			std::execution::par, image_vertical_iterator.begin(),
			image_vertical_iterator.end(), [&](int32_t y) {
				RTY_PROFILE_SCOPE("Line");
				RTY_RAYTRACER_TRACE("Scanlines remaining: {0}", y);
				std::for_each(
					std::execution::par, image_horizontal_iterator.begin(),
					image_horizontal_iterator.end(), [&](int32_t x) {
						RTY_PROFILE_SCOPE("Pixel");
						Color3 pixel_color(0);
						for (uint32_t sample = 0; sample < data.samples_per_pixel;
							++sample) {
							auto u = (float(x) + Random::RandomFloat()) / (data.width - 1);
							auto v = (float(y) + Random::RandomFloat()) / (data.height - 1);
							Ray ray = camera.ShootRay(u, v);
							pixel_color += ComputePixelColor(ray, objects, data.max_depth);
						}

						WriteColor(output_stream, pixel_color, data.samples_per_pixel);
					});
			});
#else
		// top to down
		for (int32_t j = data.height - 1; j >= 0; --j) {
			RTY_PROFILE_SCOPE("Line")
				RTY_RAYTRACER_TRACE("Scanlines remaining: {0}", j);
			// left to right
			for (int32_t i = 0; i < data.width; ++i) {
				RTY_PROFILE_SCOPE("Pixel")
					Color3 pixel_color(0);
				for (uint32_t sample = 0; sample < data.samples_per_pixel; ++sample) {
					auto u = (float(i) + Random::RandomFloat()) / (data.width - 1);
					auto v = (float(j) + Random::RandomFloat()) / (data.height - 1);
					Ray ray = camera.ShootRay(u, v);
					pixel_color += ComputePixelColor(ray, objects, data.max_depth);
				}

				WriteColor(output_stream, pixel_color, data.samples_per_pixel);
			}
		}
#endif
		output_stream.close();

		RTY_RAYTRACER_TRACE("Done.");
	}

	Color3 Raytracer::ComputePixelColor(const Ray& ray,
		const Scene& objects,
		uint32_t depth) {
		Hit hit;
		if (depth <= 0) {
			return Color3(0.0f);
		}

		hit_mutex.lock();
		auto hit_anything = objects.HitObjects(ray, 0.001f, infinity, hit);
		hit_mutex.unlock();
		if (hit_anything) {
			Ray scattered_ray;
			Color3 attenuation{};
			if (hit.material->Scatter(ray, hit, attenuation, scattered_ray)) {
				return ComputePixelColor(scattered_ray, objects, depth - 1) * attenuation;
			}
			return Color3(0.0f);
		}

		glm::vec3 unit_direction = glm::normalize(ray.GetDirection());
		auto hit_value = 0.5f * (unit_direction.y + 1.0f);
		return Color3(1.0f) * (1.0f - hit_value) +
			Color3(0.5f, 0.7f, 1.0f) * hit_value;
	}

	void Raytracer::WriteColor(std::ofstream& out, Color3 pixel_color,
		uint32_t samples_per_pixel) {
		auto scale = 1.0f / samples_per_pixel;
		pixel_color = glm::sqrt(scale * pixel_color);
		pixel_color = glm::clamp(pixel_color, 0.0f, 1.0f);

		output_mutex.lock();
		out << static_cast<int>(255.0f * pixel_color.r) << ' '
			<< static_cast<int>(255.0f * pixel_color.g) << ' '
			<< static_cast<int>(255.0f * pixel_color.b) << '\n';
		output_mutex.unlock();
	}

	void Raytracer::Shutdown() {}
}  // namespace raytracy
