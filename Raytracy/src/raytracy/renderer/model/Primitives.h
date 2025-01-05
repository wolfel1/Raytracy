#pragma once

#include <numbers>
#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>

namespace raytracy {

	struct BoundingBox {
		glm::vec3 min_corner{ infinity };
		glm::vec3 max_corner{ -infinity };
	};

	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec4 color;
		glm::vec2 tex_coords;
	};

	struct Triangle {
		std::array<shared_ptr<Vertex>, 3> vertices;
		std::array<uint32_t, 3> indices;
		glm::vec3 center;

		Triangle(shared_ptr<Vertex> vertex1, shared_ptr<Vertex> vertex2, shared_ptr<Vertex> vertex3) {
			vertices[0] = vertex1;
			vertices[1] = vertex2;
			vertices[2] = vertex3;
		}

		glm::vec3 GetCenter() {
			return (vertices[0]->position + vertices[1]->position + vertices[2]->position) / 3.0f;
		}
	};

	static const glm::vec4 DEFAULT_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };

	struct MeshData {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		std::string name = "Custom";
		bool is_indexed = false;
	};

	class MeshProvider {

	public:
		static MeshData GetPlaneData(float scale = 1.0f, glm::vec3 const& origin = {});
		static MeshData GetCubeData(float scale = 1.0f, glm::vec3 const& origin = {});
		static MeshData GetSphereData(float scale = 1.0f, glm::vec3 const& origin = {}, const uint16_t segment_count = 36, const uint16_t ring_count = 18);

	private:
		MeshProvider() = default;
	};
}