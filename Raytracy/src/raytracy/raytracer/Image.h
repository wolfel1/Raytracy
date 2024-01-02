#pragma once

#include <glm/glm.hpp>

namespace raytracy {

	class Image {
	private:
		uint32_t width;
		uint32_t height;
		uint32_t samples_per_pixel;
		uint32_t max_depth;

		glm::vec4* image_raw_data = nullptr;

	public:
		Image(uint32_t width, uint32_t height, uint32_t samples, uint32_t depth) : width(width), height(height), samples_per_pixel(samples), max_depth(depth) {}
		~Image();

		uint32_t GetWidth() const { return width; }
		uint32_t GetHeight() const { return height; }
		uint32_t GetSamplesPerPixel() const { return samples_per_pixel; }
		uint32_t GetMaxDepth() const { return max_depth; }

		void SetData(const glm::vec4* data);

		void WriteImage(const glm::vec4* data);
	};
}