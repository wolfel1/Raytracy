#include "raytracypch.h"
#include "Raytracer.h"

#include "helper/ColorHelper.h"

namespace raytracy {
	void Raytracer::Init() {

	}

	void Raytracer::Submit(const HitableCollection& objects, const Camera& camera, const ImageData& data) {
		RTY_PROFILE_FUNCTION(); 
		InstrumentationTimer t("Raytracing", true);

		std::ofstream output_stream("./image.ppm", std::ios::out | std::ios::binary);
		output_stream << "P3\n" << data.width << ' ' << data.height << "\n255\n";
		RTY_RAYTRACER_TRACE("Start process");

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
					pixel_color += ComputeRayColor(ray, objects, data.max_depth);
				}

				WriteColor(output_stream, pixel_color, data.samples_per_pixel);
			}
		}
		output_stream.close();

		RTY_RAYTRACER_TRACE("Done.");
	}

	Color3 Raytracer::ComputeRayColor(const Ray& ray, const HitableCollection& objects, uint32_t depth) {

		Hit hit;
		if (depth <= 0) {
			return Color3(0.0f);
		}

		if (objects.HitObjects(ray, 0.001f, infinity, hit)) {
			Ray scattered_ray;
			Color3 attenuation{};
			if (hit.material->Scatter(ray, hit, attenuation, scattered_ray)) {
				return ComputeRayColor(scattered_ray, objects, depth - 1) * attenuation;
			}
			return Color3(0.0f);
		}

		glm::vec3 unit_direction = glm::normalize(ray.GetDirection());
		auto hit_value = 0.5f * (unit_direction.y + 1.0f);
		return Color3(1.0f) * (1.0f - hit_value) +
			Color3(0.5f, 0.7f, 1.0f) * hit_value;
	}

	void Raytracer::WriteColor(std::ofstream& out, Color3 pixel_color, uint32_t samples_per_pixel) {

		auto red = pixel_color.r;
		auto green = pixel_color.g;
		auto blue = pixel_color.b;

		auto scale = 1.0f / samples_per_pixel;
		red = glm::sqrt(scale * red);
		green = glm::sqrt(scale * green);
		blue = glm::sqrt(scale * blue);

		out << static_cast<int>(256.0f * Clamp(red, 0.0f, 0.999f)) << ' '
			<< static_cast<int>(256.0f * Clamp(green, 0.0f, 0.999f)) << ' '
			<< static_cast<int>(256.0f * Clamp(blue, 0.0f, 0.999f)) << '\n';
	}

	void Raytracer::Shutdown() {}
}


