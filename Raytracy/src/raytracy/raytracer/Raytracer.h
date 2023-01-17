#pragma once


#include <thread>

#include "helper/Vector3.h"
#include "Ray.h"
#include "Scene.h"
#include "Camera.h"

namespace raytracy {

	struct Image {
		int32_t width;
		int32_t height;
		uint32_t samples_per_pixel;
		uint32_t max_depth;
	};

	class Raytracer {
	private:

		std::thread raytracing_thread;

		std::unique_ptr<Scene> active_scene;
		std::unique_ptr<Camera> active_camera;
		std::shared_ptr<Image> image;

	public:
		Raytracer();
		~Raytracer();
		void Submit(const Scene& objects, const Camera& camera, const Image& image_data);

	private:
		void RayTrace();
		Color3 ComputePixelColor(const Ray& ray, const Scene& objects, uint32_t depth); 
		void WriteColor(std::ofstream& out, Color3 pixel_color, uint32_t samples_per_pixel);		
	};
}