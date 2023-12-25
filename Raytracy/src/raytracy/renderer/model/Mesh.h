#pragma once

#include "../api/Shader.h"
#include "../api/Buffer.h"
#include "../api/VertexArray.h"
#include "Primitives.h"

namespace raytracy::renderer {

	class Mesh {
	private:
		shared_ptr<VertexArray> vertex_array;
		shared_ptr<Shader> shader; 
		shared_ptr<MeshData> mesh_data;

		glm::mat4 model_matrix = glm::mat4(1.0f);
		glm::vec3 origin{};
		float scale = 1.0f;

		glm::vec4 display_color = { 0.5f, 0.5f, 0.5f, 1.0f };
		glm::vec3 light_color = { 1.0f, 1.0f, 1.0f };
		glm::vec3 light_direction = { 0.0f, 2.0f, 2.0f };
		shared_ptr<UniformBuffer> shading_uniform_buffer;

	public:
		Mesh() {}
		Mesh(glm::vec3 const& position, float const scale_factor);
		virtual ~Mesh() = default;

		shared_ptr<VertexArray> GetVertexArray() const {
			return vertex_array;
		}
		shared_ptr<Shader> GetShader() const {
			return shader;
		}

		bool IsIndexed() const {
			return mesh_data->is_indexed;
		}

		glm::mat4 const& GetModelMatrix() const {
			return model_matrix;
		}

		glm::vec3 GetOrigin() const {
			return origin;
		}

		float GetScale() const {
			return scale;
		}
		
		void SetDisplayColor(glm::vec4 const& color);

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

	class Sphere : public Mesh {
	private:

	public:
		Sphere();
		Sphere(glm::vec3 const& position, float const scale_factor = 1.0f);
		~Sphere() = default;
	};
}