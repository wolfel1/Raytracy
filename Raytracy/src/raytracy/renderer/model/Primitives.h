#pragma once

#include <numbers>

namespace raytracy {

	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec4 color;
	};

	static const glm::vec4 DEFAULT_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };

	struct MeshData {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		std::string name = "Custom";
		bool is_indexed = false;

		virtual void Init() {}
	};

	struct PlaneData : public MeshData {
		virtual void Init() override {
			vertices = { { {-1.0, 0.0f, -1.0}, {0.0f, 0.f, 0.0f}, DEFAULT_COLOR},
			 { {1.0, 0.0f, -1.0}, {0.0f, 0.f, 0.0f}, DEFAULT_COLOR },
			 { {1.0, 0.0f, 1.0}, {0.0f, 0.f, 0.0f}, DEFAULT_COLOR },
			 { {-1.0, 0.0f, 1.0}, {0.0f, 0.f, 0.0f}, DEFAULT_COLOR } };

			indices = {
				2, 1, 0,
				0, 3, 2
			};
			is_indexed = true;

			name = "Plane";
		}
	};

	struct FlatCubeData : public MeshData {
		virtual void Init() override {

			vertices = {
				//positions					//normals
				//back
				{{1.0f, -1.0f, -1.0f},		{0.0f,  0.0f, -1.0f}, DEFAULT_COLOR},
				{{-1.0f, -1.0f, -1.0f,},	{0.0f,  0.0f, -1.0f}, DEFAULT_COLOR			  }	,
				{{-1.0f,  1.0f, -1.0f,},	{0.0f,  0.0f, -1.0f}, DEFAULT_COLOR			  }	,
				{{-1.0f,  1.0f, -1.0f,},	{0.0f,  0.0f, -1.0f}, DEFAULT_COLOR			  }	,
				{{1.0f,  1.0f, -1.0f,},	{0.0f,  0.0f, -1.0f}, DEFAULT_COLOR			  }	,
				{{1.0f, -1.0f, -1.0f,},	{0.0f,  0.0f, -1.0f}, DEFAULT_COLOR			  }	,

				//front													  	
				{{-1.0f, -1.0f,  1.0f},		{0.0f,  0.0f, 1.0f}, DEFAULT_COLOR			  }	,
				{{ 1.0f, -1.0f,  1.0f},		{0.0f,  0.0f, 1.0f}, DEFAULT_COLOR			  }	,
				{{ 1.0f,  1.0f,  1.0f},		{0.0f,  0.0f, 1.0f}, DEFAULT_COLOR			  }	,
				{{ 1.0f,  1.0f,  1.0f},		{0.0f,  0.0f, 1.0f}, DEFAULT_COLOR			  }	,
				{{-1.0f,  1.0f,  1.0f},		{0.0f,  0.0f, 1.0f}, DEFAULT_COLOR			  }	,
				{{-1.0f, -1.0f,  1.0f},		{0.0f,  0.0f, 1.0f}, DEFAULT_COLOR			  }	,

				//left													  	
				{{-1.0f,  -1.0f,  -1.0f },	{	-1.0f,  0.0f,  0.0f}, DEFAULT_COLOR	  }	,
				{{-1.0f,  -1.0f, 1.0f 	},	{-1.0f,  0.0f,  0.0f}, DEFAULT_COLOR		  }	,
				{{-1.0f, 1.0f, 1.0f 	},	{-1.0f,  0.0f,  0.0f}, DEFAULT_COLOR		  }	,
				{{-1.0f, 1.0f, 1.0f	},	{-1.0f,  0.0f,  0.0f}, DEFAULT_COLOR		  }	,
				{{-1.0f, 1.0f,  -1.0f 	},	{-1.0f,  0.0f,  0.0f}, DEFAULT_COLOR		  }	,
				{{-1.0f,  -1.0f,  -1.0f },	{	-1.0f,  0.0f,  0.0f}, DEFAULT_COLOR	  }	,

				//right													  	
				{ {1.0f,  -1.0f,  1.0f},		{1.0f,  0.0f,  0.0f}, DEFAULT_COLOR			  }	,
				{ {1.0f,  -1.0f, -1.0f},		{1.0f,  0.0f,  0.0f}, DEFAULT_COLOR			  }	,
				{ {1.0f, 1.0f, -1.0f	},	{1.0f,  0.0f,  0.0f}, DEFAULT_COLOR			  }	,
				{ {1.0f, 1.0f, -1.0f	},	{1.0f,  0.0f,  0.0f}, DEFAULT_COLOR			  }	,
				{ {1.0f, 1.0f,  1.0f	},	{1.0f,  0.0f,  0.0f}, DEFAULT_COLOR			  }	,
				{ {1.0f,  -1.0f,  1.0f},		{1.0f,  0.0f,  0.0f}, DEFAULT_COLOR			  }	,

				//bottom												  	
			   {{ -1.0f, -1.0f, -1.0f},		{0.0f, -1.0f,  0.0f}, DEFAULT_COLOR			  }	,
			   {{  1.0f, -1.0f, -1.0f},		{0.0f, -1.0f,  0.0f}, DEFAULT_COLOR			  }	,
			   {{  1.0f, -1.0f,  1.0f},		{0.0f, -1.0f,  0.0f}, DEFAULT_COLOR			  }	,
			   {{  1.0f, -1.0f,  1.0f},		{0.0f, -1.0f,  0.0f}, DEFAULT_COLOR			  }	,
			   {{ -1.0f, -1.0f,  1.0f},		{0.0f, -1.0f,  0.0f}, DEFAULT_COLOR			  }	,
			   {{ -1.0f, -1.0f, -1.0f},		{0.0f, -1.0f,  0.0f}, DEFAULT_COLOR			  }	,

			   //top													  	
			  {{ -1.0f,  1.0f, 1.0f	},	{0.0f,  1.0f,  0.0f}, DEFAULT_COLOR			  }	,
			  {{  1.0f,  1.0f, 1.0f	},	{0.0f,  1.0f,  0.0f}, DEFAULT_COLOR			  }	,
			  {{  1.0f,  1.0f,  -1.0f},		{0.0f,  1.0f,  0.0f}, DEFAULT_COLOR			  }	,
			  {{  1.0f,  1.0f,  -1.0f},		{0.0f,  1.0f,  0.0f}, DEFAULT_COLOR			  }	,
			  {{ -1.0f,  1.0f,  -1.0f},		{0.0f,  1.0f,  0.0f}, DEFAULT_COLOR			  }	,
			  {{ -1.0f,  1.0f, 1.0f},		{0.0f,  1.0f,  0.0f}, DEFAULT_COLOR			  }
			};


			name = "FlatCube";
		}
	};

	struct CubeData : public MeshData {
		virtual void Init() override {

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
				{{1.0f, -1.0f, -1.0f},		{1.0f,  -1.0f, -1.0f}, DEFAULT_COLOR},
				{{-1.0f, -1.0f, -1.0f,},	{-1.0f, -1.0f, -1.0f,}, DEFAULT_COLOR			  }	,
				{{-1.0f,  1.0f, -1.0f,},	{-1.0f,  1.0f, -1.0f,}, DEFAULT_COLOR			  }	,
				{{1.0f,  1.0f, -1.0f,},	{1.0f,  1.0f, -1.0f,}, DEFAULT_COLOR			  }	,

				//front													  	
				{{-1.0f, -1.0f,  1.0f},		{-1.0f, -1.0f,  1.0f}, DEFAULT_COLOR			  }	,
				{{ 1.0f, -1.0f,  1.0f},		{ 1.0f, -1.0f,  1.0f}, DEFAULT_COLOR			  }	,
				{{ 1.0f,  1.0f,  1.0f},		{ 1.0f,  1.0f,  1.0f}, DEFAULT_COLOR			  }	,
				{{-1.0f,  1.0f,  1.0f},		{-1.0f,  1.0f,  1.0f}, DEFAULT_COLOR			  }	,
			};

			is_indexed = true;
			name = "Cube";
		}
	};

	struct SphereData : public MeshData {
		virtual void Init() override {
			//vertices
			const float PI = std::numbers::pi_v<float>;
			float radius = 1.0f;
			uint32_t segment_count = 36;
			uint32_t ring_count = 18;
			float x, y, z, xz;
			float nx, ny, nz;
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

					vertices.push_back({ {x, y, z}, {nx, ny, nz}, DEFAULT_COLOR });
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