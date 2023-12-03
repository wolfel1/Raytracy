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

	struct FlatCubeData : public MeshData {
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
				{{0.5f, -0.5f, -0.5f},		{0.5f,  -0.5f, -0.5f}, DEFAULT_COLOR},
				{{-0.5f, -0.5f, -0.5f,},	{-0.5f, -0.5f, -0.5f,}, DEFAULT_COLOR			  }	,
				{{-0.5f,  0.5f, -0.5f,},	{-0.5f,  0.5f, -0.5f,}, DEFAULT_COLOR			  }	,
				{{0.5f,  0.5f, -0.5f,},	{0.5f,  0.5f, -0.5f,}, DEFAULT_COLOR			  }	,

				//front													  	
				{{-0.5f, -0.5f,  0.5f},		{-0.5f, -0.5f,  0.5f}, DEFAULT_COLOR			  }	,
				{{ 0.5f, -0.5f,  0.5f},		{ 0.5f, -0.5f,  0.5f}, DEFAULT_COLOR			  }	,
				{{ 0.5f,  0.5f,  0.5f},		{ 0.5f,  0.5f,  0.5f}, DEFAULT_COLOR			  }	,
				{{-0.5f,  0.5f,  0.5f},		{-0.5f,  0.5f,  0.5f}, DEFAULT_COLOR			  }	,
			};

			is_indexed = true;
			name = "Cube";
		}
	};
}