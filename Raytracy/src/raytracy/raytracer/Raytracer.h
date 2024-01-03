#pragma once

#include <thread>

#include <glm/glm.hpp>

namespace raytracy {

	struct Material {
		glm::vec4 color{1.0f};
		float metallic = 0.0f;
		float roughness = 0.5f;
	};

	struct Sphere {
		Material material;
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
		glm::vec3 right;
		glm::mat4 inverse_projection{};
		glm::mat4 inverse_view{};
	};

	struct Ray {
		glm::vec3 origin;
		glm::vec3 direction;
	};

	struct Hit {
		glm::vec3 point{};
		glm::vec3 normal{};
		Material material;
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
		std::thread raytracing_thread;


		glm::vec4* accumulated_color_data = nullptr;

		std::vector<uint32_t> horizontal_iterator, vertical_iterator;

	public:
		Raytracer();
		~Raytracer();
		void RaytraceScene();
		void Submit();

		private:
		void Raytrace();
	};
}