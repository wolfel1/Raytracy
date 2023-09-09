#pragma once
#include "../api/Shader.h"
#include "../api/Buffer.h"
#include "../api/VertexArray.h"

namespace raytracy {

	struct Vertex {
		glm::vec3 position;
		glm::vec4 color;
	};

	struct MeshData {
		std::vector<Vertex> vertices = { 
			{{-1.f, -1.f, 0.0f},  { 1.0f, 1.0f, 1.0f, 1.0f}},
			{{1.f, -1.f, 0.0f },  { 1.0f, 1.0f, 1.0f, 1.0f}},
			{{1.f, 1.f, 0.0f  },  { 1.0f, 1.0f, 1.0f, 1.0f}},
			{{-1.f, 1.f, 0.0f },  { 1.0f, 1.0f, 1.0f, 1.0f} }
		 };
		std::array<uint32_t, 6> indices = {
			0, 1, 2,
			2, 3, 0
		};
	};

	class Mesh {
	private:
		shared_ptr<VertexArray> vertex_array;
		shared_ptr<VertexBuffer> vertex_buffer;
		shared_ptr<IndexBuffer> index_buffer;
		shared_ptr<Shader> shader;
		std::vector<shared_ptr<UniformBuffer>> uniform_buffer;
		MeshData mesh_data{};

		public:
		Mesh();
		~Mesh();

		shared_ptr<VertexArray> GetVertexArray() const {
			return vertex_array;
		}
		shared_ptr<Shader> GetShader() const {
			return shader;
		}

		//temporar
		shared_ptr<UniformBuffer> GetUniformBuffer() const {
			return uniform_buffer[0];
		}
	};
}