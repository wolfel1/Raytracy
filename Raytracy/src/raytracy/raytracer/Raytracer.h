#pragma once

#include <thread>

#include <glm/glm.hpp>
#include "Ray.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"

namespace raytracy {

	class Raytracer {
	private:

		std::thread raytracing_thread;

		std::unique_ptr<Scene> active_scene;
		std::unique_ptr<Camera> active_camera;
		std::shared_ptr<Image> image_ptr;

		glm::vec4* accumulated_color_data = nullptr;

		std::vector<uint32_t> horizontal_iterator, vertical_iterator;

	public:
		Raytracer();
		~Raytracer();
		void Submit(const Scene& objects, const Camera& camera, const shared_ptr<Image> image);

	private:
		void RayTrace();
		glm::vec4 ComputePixelColor(const Ray& ray, const Scene& objects, uint32_t depth);
	};
}