#pragma once

#include <thread>

#include <glm/glm.hpp>
#include "Ray.h"
#include "Scene.h"
#include "Camera.h"

namespace raytracy {

	struct Image {
		uint32_t width;
		uint32_t height;
		uint32_t samples_per_pixel;
		uint32_t max_depth;
	};

	class Raytracer {
	private:

		std::thread raytracing_thread;

		std::unique_ptr<Scene> active_scene;
		std::unique_ptr<Camera> active_camera;
		std::shared_ptr<Image> image;

		glm::vec4* accumulated_color_data = nullptr;

		std::vector<uint32_t> horizontal_iterator, vertical_iterator;

	public:
		Raytracer();
		~Raytracer();
		void Submit(const Scene& objects, const Camera& camera, const Image& image_data);

	private:
		void RayTrace();
		glm::vec4 ComputePixelColor(const Ray& ray, const Scene& objects, uint32_t depth);
		void WriteImage();
	};
}