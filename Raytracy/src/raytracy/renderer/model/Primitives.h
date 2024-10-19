#pragma once

#include <numbers>

namespace raytracy {

	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec4 color;
		glm::vec2 tex_coords;
	};

	struct Triangle {
		std::array<shared_ptr<Vertex>, 3> vertices;

		Triangle(shared_ptr<Vertex> vertex1, shared_ptr<Vertex> vertex2, shared_ptr<Vertex> vertex3) {
			vertices[0] = vertex1;
			vertices[1] = vertex2;
			vertices[2] = vertex3;
		}
	};

	static const glm::vec4 DEFAULT_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };

	struct MeshData {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		std::string name = "Custom";
		bool is_indexed = false;
	};

	struct QuadData : public MeshData {
		QuadData() {
			vertices = { { {-1.0, -1.0f, 0.0}, {0.0f, 0.0f, 1.0f}, DEFAULT_COLOR, {0.0f, 0.0f}},
			 { {1.0, -1.0f, 0.0}, {0.0f, 0.0f, 1.0f}, DEFAULT_COLOR, {1.0f, 0.0f} },
			 { {1.0, 1.0f, 0.0}, {0.0f, 0.0f, 1.0f}, DEFAULT_COLOR, {1.0f, 1.0f} },
			 { {-1.0, 1.0f, 0.0}, {0.0f, 0.0f, 1.0f}, DEFAULT_COLOR, {0.0f, 1.0f} } };

			indices = {
				0, 1, 2,
				2, 3, 0
			};
			is_indexed = true;

			name = "Plane";
		}
	};

	struct FlatCubeData : public MeshData {
		FlatCubeData() {

			vertices = {
				//positions					//normals
				//back
				{{1.0f, -1.0f, -1.0f},		{0.0f,  0.0f, -1.0f}, DEFAULT_COLOR, {1.0f, 0.0f}},
				{{-1.0f, -1.0f, -1.0f,},	{0.0f,  0.0f, -1.0f}, DEFAULT_COLOR, {0.0f, 0.0f}			  }	,
				{{-1.0f,  1.0f, -1.0f,},	{0.0f,  0.0f, -1.0f}, DEFAULT_COLOR, {0.0f, 1.0f}			  }	,
				{{-1.0f,  1.0f, -1.0f,},	{0.0f,  0.0f, -1.0f}, DEFAULT_COLOR	, {0.0f, 1.0f}		  }	,
				{{1.0f,  1.0f, -1.0f,},	{0.0f,  0.0f, -1.0f}, DEFAULT_COLOR, {1.0f, 1.0f}			  }	,
				{{1.0f, -1.0f, -1.0f,},	{0.0f,  0.0f, -1.0f}, DEFAULT_COLOR, {1.0f, 0.0f}			  }	,

				//front													  	
				{{-1.0f, -1.0f,  1.0f},		{0.0f,  0.0f, 1.0f}, DEFAULT_COLOR, {0.0f, 0.0f}			  }	,
				{{ 1.0f, -1.0f,  1.0f},		{0.0f,  0.0f, 1.0f}, DEFAULT_COLOR, {1.0f, 0.0f}			  }	,
				{{ 1.0f,  1.0f,  1.0f},		{0.0f,  0.0f, 1.0f}, DEFAULT_COLOR, {1.0f, 1.0f}			  }	,
				{{ 1.0f,  1.0f,  1.0f},		{0.0f,  0.0f, 1.0f}, DEFAULT_COLOR, {1.0f, 1.0f}			  }	,
				{{-1.0f,  1.0f,  1.0f},		{0.0f,  0.0f, 1.0f}, DEFAULT_COLOR, {0.0f, 1.0f}			  }	,
				{{-1.0f, -1.0f,  1.0f},		{0.0f,  0.0f, 1.0f}, DEFAULT_COLOR, {0.0f, 0.0f}			  }	,

				//left													  	
				{{-1.0f,  -1.0f,  -1.0f },	{	-1.0f,  0.0f,  0.0f}, DEFAULT_COLOR, {0.0f, 0.0f}	  }	,
				{{-1.0f,  -1.0f, 1.0f 	},	{-1.0f,  0.0f,  0.0f}, DEFAULT_COLOR, {0.0f, 1.0f}		  }	,
				{{-1.0f, 1.0f, 1.0f 	},	{-1.0f,  0.0f,  0.0f}, DEFAULT_COLOR, {1.0f, 1.0f}		  }	,
				{{-1.0f, 1.0f, 1.0f	},	{-1.0f,  0.0f,  0.0f}, DEFAULT_COLOR, {1.0f, 1.0f}		  }	,
				{{-1.0f, 1.0f,  -1.0f 	},	{-1.0f,  0.0f,  0.0f}, DEFAULT_COLOR, {1.0f, 0.0f}		  }	,
				{{-1.0f,  -1.0f,  -1.0f },	{	-1.0f,  0.0f,  0.0f}, DEFAULT_COLOR, {0.0f, 0.0f}	  }	,

				//right													  	
				{ {1.0f,  -1.0f,  1.0f},		{1.0f,  0.0f,  0.0f}, DEFAULT_COLOR, {0.0f, 1.0f}			  }	,
				{ {1.0f,  -1.0f, -1.0f},		{1.0f,  0.0f,  0.0f}, DEFAULT_COLOR, {0.0f, 0.0f}			  }	,
				{ {1.0f, 1.0f, -1.0f	},	{1.0f,  0.0f,  0.0f}, DEFAULT_COLOR, {1.0f, 0.0f}			  }	,
				{ {1.0f, 1.0f, -1.0f	},	{1.0f,  0.0f,  0.0f}, DEFAULT_COLOR, {1.0f, 0.0f}			  }	,
				{ {1.0f, 1.0f,  1.0f	},	{1.0f,  0.0f,  0.0f}, DEFAULT_COLOR, {1.0f, 1.0f}			  }	,
				{ {1.0f,  -1.0f,  1.0f},		{1.0f,  0.0f,  0.0f}, DEFAULT_COLOR, {0.0f, 1.0f}			  }	,

				//bottom												  	
			   {{ -1.0f, -1.0f, -1.0f},		{0.0f, -1.0f,  0.0f}, DEFAULT_COLOR, {0.0f, 0.0f}			  }	,
			   {{  1.0f, -1.0f, -1.0f},		{0.0f, -1.0f,  0.0f}, DEFAULT_COLOR, {1.0f, 0.0f}			  }	,
			   {{  1.0f, -1.0f,  1.0f},		{0.0f, -1.0f,  0.0f}, DEFAULT_COLOR, {1.0f, 1.0f}			  }	,
			   {{  1.0f, -1.0f,  1.0f},		{0.0f, -1.0f,  0.0f}, DEFAULT_COLOR, {1.0f, 1.0f}			  }	,
			   {{ -1.0f, -1.0f,  1.0f},		{0.0f, -1.0f,  0.0f}, DEFAULT_COLOR, {0.0f, 1.0f}			  }	,
			   {{ -1.0f, -1.0f, -1.0f},		{0.0f, -1.0f,  0.0f}, DEFAULT_COLOR, {0.0f, 0.0f}			  }	,

			   //top													  	
			  {{ -1.0f,  1.0f, 1.0f	},	{0.0f,  1.0f,  0.0f}, DEFAULT_COLOR, {0.0f, 1.0f}			  }	,
			  {{  1.0f,  1.0f, 1.0f	},	{0.0f,  1.0f,  0.0f}, DEFAULT_COLOR, {1.0f, 1.0f}			  }	,
			  {{  1.0f,  1.0f,  -1.0f},		{0.0f,  1.0f,  0.0f}, DEFAULT_COLOR, {1.0f, 0.0f}			  }	,
			  {{  1.0f,  1.0f,  -1.0f},		{0.0f,  1.0f,  0.0f}, DEFAULT_COLOR, {1.0f, 0.0f}			  }	,
			  {{ -1.0f,  1.0f,  -1.0f},		{0.0f,  1.0f,  0.0f}, DEFAULT_COLOR, {0.0f, 0.0f}			  }	,
			  {{ -1.0f,  1.0f, 1.0f},		{0.0f,  1.0f,  0.0f}, DEFAULT_COLOR, {0.0f, 1.0f}			  }
			};


			name = "FlatCube";
		}
	};

	struct CubeData : public MeshData {
		CubeData() {

			indices = {
				//back
				0, 1, 2,
				2, 3, 0,

				//front
				4, 5, 6,
				6, 7, 4,

				//left
				1, 4, 7,
				7, 2, 1,

				//right
				5, 0, 3,
				3, 6, 5,

				//bottom
				1, 0, 5,
				5, 4, 1,

				//top
				7, 6, 3,
				3, 2, 7
			};
			vertices = {
				//positions					//normals
				//back
				{{1.0f, -1.0f, -1.0f},		{1.0f,  -1.0f, -1.0f}, DEFAULT_COLOR, {1.0f, 0.0f}},
				{{-1.0f, -1.0f, -1.0f,},	{-1.0f, -1.0f, -1.0f,}, DEFAULT_COLOR, {0.0f, 0.0f}			  }	,
				{{-1.0f,  1.0f, -1.0f,},	{-1.0f,  1.0f, -1.0f,}, DEFAULT_COLOR, {0.0f, 1.0f}			  }	,
				{{1.0f,  1.0f, -1.0f,},	{1.0f,  1.0f, -1.0f,}, DEFAULT_COLOR, {1.0f, 1.0f}			  }	,

				//front													  	
				{{-1.0f, -1.0f,  1.0f},		{-1.0f, -1.0f,  1.0f}, DEFAULT_COLOR, {0.0f, 0.0f}			  }	,
				{{ 1.0f, -1.0f,  1.0f},		{ 1.0f, -1.0f,  1.0f}, DEFAULT_COLOR, {1.0f, 0.0f}			  }	,
				{{ 1.0f,  1.0f,  1.0f},		{ 1.0f,  1.0f,  1.0f}, DEFAULT_COLOR, {1.0f, 1.0f}			  }	,
				{{-1.0f,  1.0f,  1.0f},		{-1.0f,  1.0f,  1.0f}, DEFAULT_COLOR, {0.0f, 1.0f}			  }	,
			};

			is_indexed = true;
			name = "Cube";
		}
	};

	struct SphereData : public MeshData {
		SphereData() {
			//vertices
			const float PI = std::numbers::pi_v<float>;
			float radius = 1.0f;
			uint32_t segment_count = 36;
			uint32_t ring_count = 18;
			float x, y, z, xz;
			float nx, ny, nz;
			float s, t;
			float segment_step = 2 * PI / segment_count;
			float ring_step = PI / ring_count;
			float segment_angle, ring_angle;

			for (uint32_t i = 0; i <= ring_count; i++) {
				ring_angle = PI / 2 - i * ring_step;
				xz = radius * cosf(ring_angle);
				y = radius * sinf(ring_angle);

				for (uint32_t j = 0; j <= segment_count; j++) {
					segment_angle = j * segment_step;

					x = xz * cosf(segment_angle);
					z = xz * sinf(segment_angle);

					nx = x;
					ny = y;
					nz = z;


					s = (float)j / segment_count;
					t = (float)i / ring_count;
					vertices.push_back({ {x, y, z}, {nx, ny, nz}, DEFAULT_COLOR, { s, t } });
				}
			}

			//indices
			uint32_t k1, k2;
			for (uint32_t i = 0; i < ring_count; i++) {
				k1 = i * (segment_count + 1);
				k2 = k1 + segment_count + 1;
				for (uint32_t j = 0; j < segment_count; j++, k1++, k2++) {
					if (i != 0) {
						indices.push_back(k1);
						indices.push_back(k1 + 1);
						indices.push_back(k2);
					}

					if (i != (ring_count - 1)) {
						indices.push_back(k1 + 1);
						indices.push_back(k2 + 1);
						indices.push_back(k2);
					}
				}
			}

			is_indexed = true;
			name = "Sphere";
		}
	};
}