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
			glm::vec3 root_vertex = {position.x - scale_factor/2, 0.0f, position.z - scale_factor/2};
			vertices.resize(4);
			vertices[0] = {root_vertex, WHITE};
			vertices[1] = { root_vertex + glm::vec3(scale_factor, 0.0f, 0.0f), WHITE};
			vertices[2] = { root_vertex + glm::vec3(scale_factor, 0.0f, scale_factor), WHITE };
			vertices[3] = { root_vertex + glm::vec3(0.0f, 0.0f, scale_factor), WHITE };
			
			indices = {
				0, 1, 2,
				2, 3, 0
			};

			name = "Plane";
		}
	};
}