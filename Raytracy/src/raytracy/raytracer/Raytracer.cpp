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
		const shared_ptr<Image> image) {
		active_scene = std::make_unique<Scene>(scene);
		active_camera = std::make_unique<Camera>(camera);
		image_ptr = image;
		accumulated_color_data = new glm::vec4[image_ptr->GetWidth() * image_ptr->GetHeight()];
#define MT 1
#if MT
		horizontal_iterator.resize(image_ptr->GetWidth());
		vertical_iterator.resize(image_ptr->GetHeight());
		for (uint32_t i = 0; i < image_ptr->GetWidth(); i++) {
			horizontal_iterator[i] = i;
		}
		for (uint32_t i = 0; i < image_ptr->GetHeight(); i++) {
			vertical_iterator[i] = i;
		}
#endif

		RTY_RAYTRACER_INFO("Successfully submitted!");

		raytracing_thread = std::thread{ &Raytracer::RayTrace, this };
	}

	void Raytracer::RayTrace() {
		RTY_PROFILE_FUNCTION();
		{
			InstrumentationTimer t("Raytracing", true);

			memset(accumulated_color_data, 0, image_ptr->GetWidth() * image_ptr->GetHeight() * sizeof(glm::vec4));

			RTY_RAYTRACER_TRACE("Start process");
#if MT
			std::for_each(std::execution::par, vertical_iterator.begin(), vertical_iterator.end(), [this](uint32_t y) {
				std::cout << "#";
				std::for_each(std::execution::par, horizontal_iterator.begin(), horizontal_iterator.end(), [this, y](uint32_t x) {

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
			image_ptr->SetData(accumulated_color_data);
			RTY_RAYTRACER_TRACE("Done.");
		}
	}

	glm::vec4 Raytracer::ComputePixelColor(const Ray& ray,
		const Scene& objects,
		uint32_t depth) {
#define RECURSIVE 0
#if RECURSIVE
		Hit hit;
		if (depth <= 0) {
			return glm::vec4(0.0f);
		}

		if (objects.HitObjects(ray, 0.001f, infinity, hit)) {
			Ray scattered_ray;
			glm::vec4 attenuation{};
			if (hit.material->Scatter(ray, hit, attenuation, scattered_ray)) {
				return ComputePixelColor(scattered_ray, objects, depth - 1) * attenuation;
			}
			return glm::vec4(0.0f);
		}

		glm::vec3 unit_direction = glm::normalize(ray.GetDirection());
		auto hit_value = 0.5f * (unit_direction.y + 1.0f);
		return glm::vec4(1.0f) * (1.0f - hit_value) +
			glm::vec4(0.5f, 0.7f, 1.0f, 1.0f) * hit_value;
#else
		Ray current_ray = ray;
		glm::vec4 current_attenuation(1.0f);
		for (uint32_t i = 0; i < depth; i++) {
			Hit hit;
			if (objects.HitObjects(current_ray, 0.001f, infinity, hit)) {
				Ray scattered_ray;
				glm::vec4 attenuation(1.0f);
				if (hit.material->Scatter(current_ray, hit, attenuation, scattered_ray)) {
					current_attenuation *= attenuation;
					current_ray = scattered_ray;
				} else {
					return glm::vec4(0);
				}
			} else {
				glm::vec3 unit_direction = glm::normalize(current_ray.GetDirection());
				auto hit_value = 0.5f * (unit_direction.y + 1.0f);
				glm::vec4 color = glm::vec4(1.0f) * (1.0f - hit_value) +
					glm::vec4(0.5f, 0.7f, 1.0f, 1.0f) * hit_value;
				return current_attenuation * color;
			}
		}
		return glm::vec4(0);
#endif
	}
}  // namespace raytracy
