#pragma once

namespace raytracy {

	struct Vertex {
		glm::vec3 position;
		glm::vec4 color;
	};

	static const glm::vec4 WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };

	struct MeshData {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		glm::vec4 display_color = { 0.5f, 0.5f, 0.5f, 1.0f };
		std::string name = "Custom";

		virtual void Init(glm::vec3 position = { 0.0f, 0.0f, 0.0f }, float scale_factor = 1.f) {}
	};

	struct PlaneData : public MeshData {
		virtual void Init(glm::vec3 position = {0.0f, 0.0f, 0.0f}, float scale_factor = 1.f) override {
			float translation = scale_factor / 2;
			glm::vec3 root_vertex = {position.x - scale_factor/2, 0.0f, position.z - scale_factor/2};
			vertices.resize(4);
			/*vertices[0] = {position + glm::vec3(-translation, 0.0f, -translation), WHITE};
			vertices[1] = { position + glm::vec3(translation, 0.0f, -translation), WHITE};
			vertices[2] = { position + glm::vec3(translation, 0.0f, translation), WHITE };
			vertices[3] = { position + glm::vec3(-translation, 0.0f, translation), WHITE };*/

			uint16_t i = 0;
			int16_t x = -1;
			for (int16_t y = -1; y <= 1; y += 2) {
				for (int16_t j = 0; j <= 1; j++) {
					vertices[i++] = { position + glm::vec3(x * translation, 0.0f, translation*y), WHITE };
					x *= -1;
				}
				x = 1;
			}
			
			indices = {
				0, 1, 2,
				2, 3, 0
			};

			name = "Plane";
		}
	};
}