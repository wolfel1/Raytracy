#pragma once


#include <thread>

#include "helper/Vector3.h"
#include "Ray.h"
#include "Scene.h"
#include "Camera.h"

namespace raytracy {

	struct ImageData {
		int32_t width;
		int32_t height;
		uint32_t samples_per_pixel;
		uint32_t max_depth;
	};

	class Raytracer {
	private:

		std::thread raytracing_thread;

	public:
		Raytracer();
		~Raytracer();

		void Submit(const Scene& objects, const Camera& camera, const ImageData& image_data);

	private:
		Color3 ComputePixelColor(const Ray& ray, const Scene& objects, uint32_t depth); 
		
		void WriteColor(std::ofstream& out, Color3 pixel_color, uint32_t samples_per_pixel);		
	};
}