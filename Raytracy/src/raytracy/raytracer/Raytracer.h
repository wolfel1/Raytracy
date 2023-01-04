#pragma once

#include "helper/Vector3.h"
#include "Ray.h"
#include "HitableCollection.h"
#include "Camera.h"

namespace raytracy {

	struct ImageData {
		int32_t width;
		int32_t height;
		uint32_t samples_per_pixel;
		uint32_t max_depth;
	};

	class Raytracer {
	public:
		static void Init();

		static void Submit(const HitableCollection& objects, const Camera& camera, const ImageData& image_data);

		static void Shutdown();

	private:
		static Color3 ComputeRayColor(const Ray& ray, const HitableCollection& objects, uint32_t depth); 
		
		static void WriteColor(std::ofstream& out, Color3 pixel_color, uint32_t samples_per_pixel);
		
		
	};
}