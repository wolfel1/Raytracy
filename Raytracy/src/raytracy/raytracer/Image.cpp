#include "raytracypch.h"
#include "Image.h"

namespace raytracy {

	Image::~Image() {
		delete[] image_raw_data;
	}

	void Image::SetData(const glm::vec4* data) {
		image_raw_data = new glm::vec4[width * height];
		std::copy(data, data + width * height, image_raw_data);
	}

	void Image::WriteImage() {
		RTY_PROFILE_FUNCTION();
		RTY_BASE_ASSERT(image_raw_data, "No image data!");

		if (image_raw_data) {

			std::ofstream output_stream("./image.ppm", std::ios::out | std::ios::binary);
			output_stream << "P3\n" << width << ' ' << height << "\n255\n";
			RTY_RAYTRACER_TRACE("Save as PPM...");

			for (int32_t j = (int32_t)height - 1; j >= 0; j--) {
				// left to right
				for (int32_t i = 0; i < (int32_t)width; i++) {
					glm::vec4 pixel_color = image_raw_data[i + j * width];

					output_stream << static_cast<int>(255.0f * pixel_color.r) << ' '
						<< static_cast<int>(255.0f * pixel_color.g) << ' '
						<< static_cast<int>(255.0f * pixel_color.b) << '\n';
				}
			}

			output_stream.close();
			RTY_RAYTRACER_TRACE("Done.");

		}
	}
}
