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

		glm::mat4 model_matrix = glm::mat4(1.0f);
		glm::vec3 origin{};
		glm::vec4 display_color = { 0.5f, 0.5f, 0.5f, 1.0f };

	public:
		Mesh() {}
		Mesh(glm::vec3 const& position);
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

		glm::mat4 const& GetModelMatrix() const {
			return model_matrix;
		}
		
		void SetDisplayColor(glm::vec4 const& color) {
			display_color = color;
			shader->Bind();
			auto& uniform_buffers = shader->GetUniformBuffers();
			auto it = uniform_buffers.find("shading");
			RTY_ASSERT(it != uniform_buffers.end(), "No uniform buffer with key 'shading' exists!");
			it->second->SetVec4("color", color);
			shader->Unbind();
		}

		void Translate(glm::vec3 const& direction);

		void Scale(float const value);

		protected:
			void Init(shared_ptr<MeshData> const mesh_data);
	};

	class Plane : public Mesh {
	private:

	public:
		Plane();
		Plane(glm::vec3 const& position, float const scale_factor = 1.0f);
		~Plane() = default;
	};

	class Cube : public Mesh {
	private:

	public:
		Cube();
		Cube(glm::vec3 const& position, float const scale_factor = 1.0f);
		~Cube() = default;
	};
}