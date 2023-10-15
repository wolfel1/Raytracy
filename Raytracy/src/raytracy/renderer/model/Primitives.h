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
			float half_size = scale_factor / 2;
			glm::vec3 root_vertex = {position.x - half_size, 0.0f, position.z - half_size};
			vertices.resize(4);
			/*vertices[0] = {position + glm::vec3(-half_size, 0.0f, -half_size), WHITE};
			vertices[1] = { position + glm::vec3(half_size, 0.0f, -half_size), WHITE};
			vertices[2] = { position + glm::vec3(half_size, 0.0f, half_size), WHITE };
			vertices[3] = { position + glm::vec3(-half_size, 0.0f, half_size), WHITE };*/

			uint16_t i = 0;
			int16_t x = -1;
			for (int16_t y = -1; y <= 1; y += 2) {
				for (int16_t j = 0; j <= 1; j++) {
					vertices[i++] = { position + glm::vec3(x * half_size, 0.0f, half_size*y), WHITE };
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

	struct CubeData : public MeshData {
		virtual void Init(glm::vec3 position = { 0.0f, 0.0f, 0.0f }, float scale_factor = 1.f) override {
			float half_size = scale_factor / 2;
			glm::vec3 root_vertex = { position.x - half_size, position.y - half_size, position.z - half_size };
			vertices.resize(8);
			vertices[0] = { position + glm::vec3(-half_size, -half_size, half_size), WHITE };
			vertices[1] = { position + glm::vec3(half_size, -half_size, half_size), WHITE };
			vertices[2] = { position + glm::vec3(half_size, half_size, half_size), WHITE };
			vertices[3] = { position + glm::vec3(-half_size, half_size, half_size), WHITE };

			vertices[4] = {position + glm::vec3(-half_size, -half_size, -half_size), WHITE};
			vertices[5] = { position + glm::vec3(half_size, -half_size, -half_size), WHITE};
			vertices[6] = { position + glm::vec3(half_size, half_size, -half_size), WHITE };
			vertices[7] = { position + glm::vec3(-half_size, half_size, -half_size), WHITE };


			/*uint16_t i = 0;
			int16_t x = -1;
			for (int16_t y = -1; y <= 1; y += 2) {
				for (int16_t j = 0; j <= 1; j++) {
					vertices[i++] = { position + glm::vec3(x * half_size, 0.0f, half_size * y), WHITE };
					x *= -1;
				}
				x = 1;
			}*/

			indices = {
				//front
				0, 1, 2,
				2, 3, 0,

				//left
				7, 4, 0,
				0, 3, 7,

				//bottom
				5, 1, 0,
				0, 4, 5,


				//right
				6, 2, 1,
				1, 5, 6,

				//back
				4, 7, 6,
				6, 5, 4,

				//top
				3, 2, 6,
				6, 7, 3
			};

			name = "Cube";
		}
	};
}