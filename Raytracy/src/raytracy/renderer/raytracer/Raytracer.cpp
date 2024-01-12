#include "raytracypch.h"
#include "Raytracer.h"

#include <execution>

#include "../../Application.h"

#include <glad/gl.h>

namespace raytracy {

	GLuint scene_buffer;

	void Raytracer::Preprocess() {
		/*const auto aspect_ratio = 16.0f / 9.0f;

		image.width = 720;
		image.height = static_cast<uint32_t>(image.width / aspect_ratio);
		image.samples = 50;
		image.max_depth = 50;

		Material world_material;
		world_material.color = glm::vec4(0.2f, 1.0f, 0.2f, 1.0f);
		scene.spheres.push_back({ world_material, glm::vec3(0.0f, -101.0f, 0.0f), 100.0f });

		Material diffuse_material;
		diffuse_material.color = glm::vec4(1.0f, 0.2f, 0.2f, 1.0f);

		scene.spheres.push_back({ diffuse_material, glm::vec3(0.0f, 0.0f, 0.0f), 0.75f });

		Material metal_material;
		metal_material.color = glm::vec4(1.0f, 1.0f, 0.2f, 1.0f);
		metal_material.metallic = 1.0f;

		scene.spheres.push_back({ metal_material, glm::vec3(2.0f, 0.0f, 0.0f), 0.75f });

		Material dielectric_material;
		dielectric_material.roughness = 0.0f;

		scene.spheres.push_back({ dielectric_material, glm::vec3(-2.0f, 0.0f, 0.0f), 0.75f });

		glm::vec3 position({ 0.0f, 0.0f, 5.0f });
		glm::vec3 direction({ 0.0f, 0.0f, -1.0f });
		glm::vec3 right{ 1.0f, 0.0f, 0.0f };
		glm::vec3 up{ 0.0f, 1.0f, 0.0f };*/

		auto& app_spec = Application::Get().GetSpecification();
		image.width = app_spec.width;
		image.height = app_spec.height;
		image.samples = 50;
		image.max_depth = 50;

		auto viewport_scene = renderer::Scene::Get();
		auto viewport_camera = viewport_scene->GetCamera();
		for (auto& mesh : viewport_scene->GetMeshes()) {

			Material material;
			material.color = mesh->GetMaterial()->GetColor();

			//scene.spheres.push_back({mesh->GetOrigin(), mesh->GetScale(),
			//										 material});
		}

		camera.position = viewport_camera->GetPosition();
		camera.direction = viewport_camera->GetDirection();
		camera.up = viewport_camera->GetUp();

		camera.inverse_projection = glm::inverse(viewport_camera->GetProjectionMatrix());
		camera.inverse_view = glm::inverse(viewport_camera->GetViewMatrix());
		ray_directions.resize(image.width * image.height);
		for (uint32_t y = 0; y < image.height; y++)
		{
			for (uint32_t x = 0; x < image.width; x++)
			{
				glm::vec2 coord = { (float)x / (float)image.width, (float)y / (float)image.height };
				coord = coord * 2.0f - 1.0f; // -1 -> 1

				glm::vec4 target = camera.inverse_projection * glm::vec4(coord.x, coord.y, 1, 1);
				glm::vec3 rayDirection = glm::vec3(camera.inverse_view * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0)); // World space
				ray_directions[x + y * image.width] = rayDirection;
			}
		}

	}

	void Raytracer::Init(shared_ptr<OpenGLRendererAPI> const api) {
		renderer_api = api;

		auto& app_spec = Application::Get().GetSpecification();
		raytracing_canvas = OpenGLTexture2D::Create(app_spec.width, app_spec.height, GL_RGBA32F);
		raytracing_kernel = ShaderLibrary::Get().Load("raytrace_kernel");
		raytracing_output = ShaderLibrary::Get().Load("raytrace_output");

		glCreateBuffers(1, &scene_buffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, scene_buffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, 8192, NULL, GL_DYNAMIC_COPY);

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, scene_buffer);

		Sphere sphere = {{1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, 1.0f};
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(Sphere), &sphere);
	}

	void Raytracer::Raytrace(shared_ptr<renderer::Scene> const scene) {
		renderer_api->ClearViewport();

		raytracing_kernel->Bind();
		raytracing_canvas->BindImage();
		renderer_api->LaunchComputeShader(raytracing_canvas->GetWidth(), raytracing_canvas->GetHeight(), 1);

		renderer_api->SetMemoryBarrier();

		raytracing_canvas->Bind(0);
		raytracing_output->Bind();
		raytracing_output->SetInt("tex", 0);
		renderer_api->Draw(4);
	}

	void Raytracer::Submit() {
		Preprocess();

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


		WriteImage(accumulated_color_data);
	}

	void Raytracer::RayTrace() {
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
						Ray ray{};
						ray.origin = camera.position;
						ray.direction = ray_directions[x + y * image.width];
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

	

	glm::vec4 Raytracer::ComputePixelColor(const Ray& ray) {
		Ray current_ray = ray;
		glm::vec4 current_attenuation(1.0f);
		for (uint32_t i = 0; i < image.max_depth; i++) {
			Hit hit;
			if (Trace(current_ray, 0.001f, infinity, hit)) {
				auto scatter_direction = hit.normal + Random::RandomUnitVector();

				if (glm::all(glm::lessThan(glm::abs(scatter_direction), glm::vec3(1e-8f)))) {
					scatter_direction = hit.normal;
				}

				current_ray = Ray(hit.point, scatter_direction);
				current_attenuation *= hit.material.color;
			} else {
				glm::vec3 unit_direction = glm::normalize(current_ray.direction);
				auto hit_value = 0.5f * (unit_direction.y + 1.0f);
				glm::vec4 color(0.1f, 0.1f, 0.1f, 1.0f);
				return current_attenuation * color;
			}
		}
		return glm::vec4(0);
	}

	void Raytracer::WriteImage(const glm::vec4* data) {
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


	bool Raytracer::HitSphere(const Ray& ray, float min, float max, Hit& hit, Sphere const& sphere) {
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
		glm::vec3 outward_normal = (hit.point - sphere.origin) / sphere.radius;
		hit.SetFaceNormal(ray, outward_normal);
		//hit.material = sphere.material;
		return true;
	}

	bool Raytracer::Trace(const Ray& ray, float min, float max, Hit& hit) {
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


}  // namespace raytracy
