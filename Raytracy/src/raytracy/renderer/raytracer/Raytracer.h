#pragma once

#include <thread>

#include "../camera/PerspectiveCamera.h"
#include "../api/opengl/OpenGLTexture.h"
#include "../api/opengl/OpenGLRendererAPI.h"
#include "../api/opengl/OpenGLShader.h"
#include "../ViewportScene.h"
#include <glm/glm.hpp>

namespace raytracy {

	struct Material {
		glm::vec4 color{ 1.0f };
		float metallic = 0.0f;
		float roughness = 0.5f;
	};

	struct Sphere {
		glm::vec4 color;
		glm::vec3 origin;
		float radius;
	};

	struct Scene {
		std::vector<Sphere> spheres;
	};

	struct Camera {
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 up;
		glm::mat4 inverse_projection;
		glm::mat4 inverse_view;
	};

	struct Ray {
		glm::vec3 origin;
		glm::vec3 direction;
	};

	struct Hit {
		glm::vec3 point{};
		glm::vec3 normal{};
		glm::vec4 color{};
		float hit_value{};
		bool front_face{};

		inline void SetFaceNormal(const Ray& ray, const glm::vec3& outward_normal) {
			front_face = glm::dot(ray.direction, outward_normal) < 0;
			normal = glm::normalize(front_face ? outward_normal : -outward_normal);
		}
	};

	struct Image {
		uint32_t width;
		uint32_t height;
		uint32_t samples;
		uint32_t max_depth;
	};

	class Raytracer {
	private:
		Scene scene{};
		Camera camera{};
		Image image{};


		glm::vec4* accumulated_color_data = nullptr;

		std::vector<uint32_t> horizontal_iterator, vertical_iterator;
		std::vector<glm::vec3> ray_directions;

		shared_ptr<OpenGLRendererAPI> renderer_api = nullptr;

		shared_ptr<OpenGLTexture2D> raytracing_canvas = nullptr;

		shared_ptr<OpenGLShader> raytracing_kernel;
		shared_ptr<OpenGLShader> raytracing_output;

		shared_ptr<OpenGLUniformBuffer> scene_data_uniform_buffer;

	public:
		Raytracer(const Raytracer&) = delete;
		~Raytracer() = default;

		static Raytracer& Get() {
			static Raytracer raytracer;
			return raytracer;
		}
		void Init(shared_ptr<OpenGLRendererAPI> const renderer_api);
		void Raytrace(shared_ptr<renderer::Scene> const scene);

		void Shutdown();

	private:
		Raytracer() {}

		void Submit();
		void Preprocess(shared_ptr<renderer::Scene> const scene);
		void RayTrace();
		glm::vec4 ComputePixelColor(const Ray& ray);
		void WriteImage(const glm::vec4* data);
		bool HitSphere(const Ray& ray, float min, float max, Hit& hit, Sphere const& sphere);
		bool Trace(const Ray& ray, float min, float max, Hit& hit);
	};
}