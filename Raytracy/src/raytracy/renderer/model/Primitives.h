#pragma once

namespace raytracy {

	enum class Primitive {
		Custom = 0, Plane, Cube
	};
	struct Vertex {
		glm::vec3 position;
		glm::vec4 color;
	};

	struct MeshData {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		glm::vec4 display_color = { 0.5f, 0.5f, 0.5f, 1.0f };
		std::string name = "Custom";

		virtual void Init() {}
	};

	struct PlaneData : public MeshData {
		void Init() {
			vertices = {
				{{-0.5f, -0.5f, 0.0f},  { 1.0f, 1.0f, 1.0f, 1.0f}},
				{{0.5f, -0.5f, 0.0f },  { 1.0f, 1.0f, 1.0f, 1.0f}},
				{{0.5f, 0.5f, 0.0f  },  { 1.0f, 1.0f, 1.0f, 1.0f}},
				{{-0.5f, 0.5f, 0.0f },  { 1.0f, 1.0f, 1.0f, 1.0f} }
			};
			indices = {
				0, 1, 2,
				2, 3, 0
			};

			name = "Plane";
		}
	};
}