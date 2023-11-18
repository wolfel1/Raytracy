#pragma once

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
			vertices = { { {-0.5, 0.0f, -0.5}, {0.0f, 0.f, 0.0f}, DEFAULT_COLOR},
			 { {0.5, 0.0f, -0.5}, {0.0f, 0.f, 0.0f}, DEFAULT_COLOR },
			 { {0.5, 0.0f, 0.5}, {0.0f, 0.f, 0.0f}, DEFAULT_COLOR },
			 { {-0.5, 0.0f, 0.5}, {0.0f, 0.f, 0.0f}, DEFAULT_COLOR } };

			indices = {
				2, 1, 0,
				0, 3, 2
			};
			is_indexed = true;

			name = "Plane";
		}
	};

	struct CubeData : public MeshData {
		virtual void Init() override {

			vertices = {
				//positions					//normals
				//back
				{{0.5f, -0.5f, -0.5f},		{0.0f,  0.0f, -1.0f}, DEFAULT_COLOR},
				{{-0.5f, -0.5f, -0.5f,},	{0.0f,  0.0f, -1.0f}, DEFAULT_COLOR			  }	,
				{{-0.5f,  0.5f, -0.5f,},	{0.0f,  0.0f, -1.0f}, DEFAULT_COLOR			  }	,
				{{-0.5f,  0.5f, -0.5f,},	{0.0f,  0.0f, -1.0f}, DEFAULT_COLOR			  }	,
				{{0.5f,  0.5f, -0.5f,},	{0.0f,  0.0f, -1.0f}, DEFAULT_COLOR			  }	,
				{{0.5f, -0.5f, -0.5f,},	{0.0f,  0.0f, -1.0f}, DEFAULT_COLOR			  }	,

				//front													  	
				{{-0.5f, -0.5f,  0.5f},		{0.0f,  0.0f, 1.0f}, DEFAULT_COLOR			  }	,
				{{ 0.5f, -0.5f,  0.5f},		{0.0f,  0.0f, 1.0f}, DEFAULT_COLOR			  }	,
				{{ 0.5f,  0.5f,  0.5f},		{0.0f,  0.0f, 1.0f}, DEFAULT_COLOR			  }	,
				{{ 0.5f,  0.5f,  0.5f},		{0.0f,  0.0f, 1.0f}, DEFAULT_COLOR			  }	,
				{{-0.5f,  0.5f,  0.5f},		{0.0f,  0.0f, 1.0f}, DEFAULT_COLOR			  }	,
				{{-0.5f, -0.5f,  0.5f},		{0.0f,  0.0f, 1.0f}, DEFAULT_COLOR			  }	,

				//left													  	
				{{-0.5f,  -0.5f,  -0.5f },	{	-1.0f,  0.0f,  0.0f}, DEFAULT_COLOR	  }	,
				{{-0.5f,  -0.5f, 0.5f 	},	{-1.0f,  0.0f,  0.0f}, DEFAULT_COLOR		  }	,
				{{-0.5f, 0.5f, 0.5f 	},	{-1.0f,  0.0f,  0.0f}, DEFAULT_COLOR		  }	,
				{{-0.5f, 0.5f, 0.5f	},	{-1.0f,  0.0f,  0.0f}, DEFAULT_COLOR		  }	,
				{{-0.5f, 0.5f,  -0.5f 	},	{-1.0f,  0.0f,  0.0f}, DEFAULT_COLOR		  }	,
				{{-0.5f,  -0.5f,  -0.5f },	{	-1.0f,  0.0f,  0.0f}, DEFAULT_COLOR	  }	,

				//right													  	
				{ {0.5f,  -0.5f,  0.5f},		{1.0f,  0.0f,  0.0f}, DEFAULT_COLOR			  }	,
				{ {0.5f,  -0.5f, -0.5f},		{1.0f,  0.0f,  0.0f}, DEFAULT_COLOR			  }	,
				{ {0.5f, 0.5f, -0.5f	},	{1.0f,  0.0f,  0.0f}, DEFAULT_COLOR			  }	,
				{ {0.5f, 0.5f, -0.5f	},	{1.0f,  0.0f,  0.0f}, DEFAULT_COLOR			  }	,
				{ {0.5f, 0.5f,  0.5f	},	{1.0f,  0.0f,  0.0f}, DEFAULT_COLOR			  }	,
				{ {0.5f,  -0.5f,  0.5f},		{1.0f,  0.0f,  0.0f}, DEFAULT_COLOR			  }	,

				//bottom												  	
			   {{ -0.5f, -0.5f, -0.5f},		{0.0f, -1.0f,  0.0f}, DEFAULT_COLOR			  }	,
			   {{  0.5f, -0.5f, -0.5f},		{0.0f, -1.0f,  0.0f}, DEFAULT_COLOR			  }	,
			   {{  0.5f, -0.5f,  0.5f},		{0.0f, -1.0f,  0.0f}, DEFAULT_COLOR			  }	,
			   {{  0.5f, -0.5f,  0.5f},		{0.0f, -1.0f,  0.0f}, DEFAULT_COLOR			  }	,
			   {{ -0.5f, -0.5f,  0.5f},		{0.0f, -1.0f,  0.0f}, DEFAULT_COLOR			  }	,
			   {{ -0.5f, -0.5f, -0.5f},		{0.0f, -1.0f,  0.0f}, DEFAULT_COLOR			  }	,

			   //top													  	
			  {{ -0.5f,  0.5f, 0.5f	},	{0.0f,  1.0f,  0.0f}, DEFAULT_COLOR			  }	,
			  {{  0.5f,  0.5f, 0.5f	},	{0.0f,  1.0f,  0.0f}, DEFAULT_COLOR			  }	,
			  {{  0.5f,  0.5f,  -0.5f},		{0.0f,  1.0f,  0.0f}, DEFAULT_COLOR			  }	,
			  {{  0.5f,  0.5f,  -0.5f},		{0.0f,  1.0f,  0.0f}, DEFAULT_COLOR			  }	,
			  {{ -0.5f,  0.5f,  -0.5f},		{0.0f,  1.0f,  0.0f}, DEFAULT_COLOR			  }	,
			  {{ -0.5f,  0.5f, 0.5f},		{0.0f,  1.0f,  0.0f}, DEFAULT_COLOR			  }
			};

			name = "Cube";
		}
	};
}