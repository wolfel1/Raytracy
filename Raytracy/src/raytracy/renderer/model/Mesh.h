#pragma once

#include "../api/Shader.h"
#include "../api/Buffer.h"
#include "../api/VertexArray.h"
#include "Primitives.h"

namespace raytracy {

	class Mesh {
	private:
		shared_ptr<VertexArray> vertex_array;
		shared_ptr<VertexBuffer> vertex_buffer;
		shared_ptr<IndexBuffer> index_buffer;
		shared_ptr<Shader> shader; 
		shared_ptr<MeshData> mesh_data;

	public:
		Mesh() {}
		virtual ~Mesh() = default;


		shared_ptr<VertexArray> GetVertexArray() const {
			return vertex_array;
		}
		shared_ptr<Shader> GetShader() const {
			return shader;
		}

		shared_ptr<MeshData> GetData() const {
			return mesh_data;
		}

		protected:
			void Init(shared_ptr<MeshData> const mesh_data);

	};

	class Plane : public Mesh {
	private:

	public:
		Plane();
		~Plane() = default;
	};
}