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
		std::array<Vertex, 3> vertices;
		glm::vec3 center;
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