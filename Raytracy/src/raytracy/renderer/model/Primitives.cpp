#include "raytracypch.h"
#include "Primitives.h"

namespace raytracy {
	MeshData MeshProvider::GetPlaneData(float scale, glm::vec3 const& origin) {
		MeshData plane;
		plane.name = "Plane";

		float halfSize = scale / 2.0f;

		glm::vec3 normal = { 0.0f, 1.0f, 0.0f };
		plane.vertices = {
			{
				{origin.x - halfSize, origin.y, origin.z + halfSize},
				normal,
				DEFAULT_COLOR,
				{0.0f, 0.0f}
			},
			{
				{origin.x + halfSize, origin.y, origin.z + halfSize},
				normal,
				DEFAULT_COLOR,
				{1.0f, 0.0f}
			},
			{
				{origin.x + halfSize, origin.y, origin.z - halfSize},
				normal,
				DEFAULT_COLOR,
				{1.0f, 1.0f}
			},
			{
				{origin.x - halfSize, origin.y, origin.z - halfSize},
				normal,
				DEFAULT_COLOR,
				{0.0f, 1.0f}
			}
		};

		plane.indices = {
			0, 1, 2,
			2, 3, 0
		};
		plane.is_indexed = true;

		return plane;
	}

	MeshData MeshProvider::GetCubeData(float scale, glm::vec3 const& origin) {
		MeshData cube;
		float halfSize = scale / 2.0f;

		const std::array<glm::vec3, 8> corners = { {
			{-1, -1, -1},
			{ 1, -1, -1},
			{ 1,  1, -1},
			{-1,  1, -1},
			{-1, -1,  1},
			{ 1, -1,  1},
			{ 1,  1,  1},
			{-1,  1,  1}
		} };

		cube.vertices.reserve(8);
		for (const auto& corner : corners) {
			Vertex& vertex = cube.vertices.emplace_back();
			vertex.position = origin + halfSize * corner;
			vertex.normal = glm::normalize(corner);
			vertex.color = DEFAULT_COLOR;
			vertex.tex_coords = (corner + 1.0f) * 0.5f;
		}

		cube.indices = {
			4, 5, 6,
			4, 6, 7,

			1, 0, 3,
			1, 3, 2,

			5, 1, 2,
			5, 2, 6,

			0, 4, 7,
			0, 7, 3,

			7, 6, 2,
			7, 2, 3,

			0, 1, 5,
			0, 5, 4
		};
		cube.is_indexed = true;
		cube.name = "Cube";

		return cube;
	}

	MeshData MeshProvider::GetSphereData(float scale, glm::vec3 const& origin, const uint16_t segment_count, const uint16_t ring_count) {
		MeshData sphere;
		sphere.name = "Sphere";

		const float radius = scale * 0.5f;
		const int vertex_count = (ring_count + 1) * (segment_count + 1);

		sphere.vertices.reserve(vertex_count);
		for (int i = 0; i <= ring_count; ++i) {
			float stack_angle = glm::pi<float>() / 2 - i * glm::pi<float>() / ring_count;
			float xz = radius * cosf(stack_angle);
			float y = radius * sinf(stack_angle);

			for (int j = 0; j <= segment_count; ++j) {
				float sectorAngle = j * 2 * glm::pi<float>() / segment_count;

				Vertex& vertex = sphere.vertices.emplace_back();
				vertex.position = origin + glm::vec3(xz * cosf(sectorAngle), y, xz * sinf(sectorAngle));
				vertex.normal = glm::normalize(vertex.position - origin);
				vertex.color = DEFAULT_COLOR;
				vertex.tex_coords = glm::vec2((float)j / segment_count, (float)i / ring_count);
			}
		}
		sphere.vertices.shrink_to_fit();

		for (int i = 0; i < ring_count; ++i) {
			int k1 = i * (segment_count + 1);
			int k2 = k1 + segment_count + 1;

			for (int j = 0; j < segment_count; ++j, ++k1, ++k2) {
				if (i != 0) {
					sphere.indices.emplace_back(k1);
					sphere.indices.emplace_back(k1 + 1);
					sphere.indices.emplace_back(k2);
				}

				if (i != (ring_count - 1)) {
					sphere.indices.emplace_back(k1 + 1);
					sphere.indices.emplace_back(k2 + 1);
					sphere.indices.emplace_back(k2);
				}
			}
		}

		sphere.is_indexed = true;
		return sphere;
	}
}

