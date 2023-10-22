#pragma once

namespace raytracy {

	struct Vertex {
		glm::vec3 position;
		glm::vec4 color;
	};

	static const glm::vec4 DEFAULT_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };

	struct MeshData {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		std::string name = "Custom";

		virtual void Init(glm::vec3 position = { 0.0f, 0.0f, 0.0f }, float scale_factor = 1.f) {}
	};

	struct PlaneData : public MeshData {
		virtual void Init(glm::vec3 position = {0.0f, 0.0f, 0.0f}, float scale_factor = 1.f) override {
			float half_size = scale_factor / 2;
			vertices.resize(4);
			vertices[0] = {position + glm::vec3(-half_size, 0.0f, -half_size), DEFAULT_COLOR};
			vertices[1] = { position + glm::vec3(half_size, 0.0f, -half_size), DEFAULT_COLOR};
			vertices[2] = { position + glm::vec3(half_size, 0.0f, half_size), DEFAULT_COLOR };
			vertices[3] = { position + glm::vec3(-half_size, 0.0f, half_size), DEFAULT_COLOR };

			
			indices = {
				2, 1, 0,
				0, 3, 2
			};

			name = "Plane";
		}
	};

	struct CubeData : public MeshData {
		virtual void Init(glm::vec3 position = { 0.0f, 0.0f, 0.0f }, float scale_factor = 1.f) override {
			float half_size = scale_factor / 2;
			vertices.resize(8);
			vertices[0] = { position + glm::vec3(-half_size, -half_size, half_size), DEFAULT_COLOR };
			vertices[1] = { position + glm::vec3(half_size, -half_size, half_size), DEFAULT_COLOR };
			vertices[2] = { position + glm::vec3(half_size, half_size, half_size), DEFAULT_COLOR };
			vertices[3] = { position + glm::vec3(-half_size, half_size, half_size), DEFAULT_COLOR };

			vertices[4] = {position + glm::vec3(-half_size, -half_size, -half_size), DEFAULT_COLOR};
			vertices[5] = { position + glm::vec3(half_size, -half_size, -half_size), DEFAULT_COLOR};
			vertices[6] = { position + glm::vec3(half_size, half_size, -half_size), DEFAULT_COLOR };
			vertices[7] = { position + glm::vec3(-half_size, half_size, -half_size), DEFAULT_COLOR };

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