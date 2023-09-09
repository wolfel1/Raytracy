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
		std::vector<shared_ptr<UniformBuffer>> uniform_buffer;

	public:
		Mesh(Primitive const& base_type);
		~Mesh() = default;

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