#pragma once

#include "../api/opengl/OpenGLBuffer.h"
#include "../api/opengl/OpenGLVertexArray.h"
#include "../api/opengl/OpenGLRendererAPI.h"
#include "Primitives.h"
#include "Material.h"

namespace raytracy::renderer {

	

	class Mesh {
	protected:
		shared_ptr<OpenGLVertexArray> vertex_array;
		shared_ptr<Material> material;

		bool is_indexed = false;

		glm::mat4 model_matrix = glm::mat4(1.0f);
		glm::vec3 origin{};
		float scale = 1.0f;

	public:
		Mesh() {}
		Mesh(glm::vec3 const& position, float const scale_factor);
		virtual ~Mesh();

		shared_ptr<Material> GetMaterial() const {
			return material;
		}

		void SetMaterial(shared_ptr<Material> material) {
			this->material = material;
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

		virtual void Draw(shared_ptr<OpenGLRendererAPI> api);

		void Translate(glm::vec3 const& direction);

		void Rotate(glm::vec3 const& axis, float const value);

		void Scale(float const value);

		protected:
			void Init(shared_ptr<MeshData> const mesh_data);
	};

	class Plane : public Mesh {
	private:
		static QuadData data;

	public:
		Plane(glm::vec3 const position = {}, float const scale_factor = 1.0f);
		~Plane() = default;
	};

	class Cube : public Mesh {
	private:
		static CubeData data;

	public:
		Cube(glm::vec3 const position = {}, float const scale_factor = 1.0f);
		~Cube() = default;
	};

	class Sphere : public Mesh {
	private:
		static SphereData data;

	public:
		Sphere(glm::vec3 const position = {}, float const scale_factor = 1.0f);
		~Sphere() = default;
	};

	class Skybox : public Mesh {
	private:
		static CubeData data;

	public:
		Skybox(glm::vec3 const position = {}, float const scale_factor = 1.0f);
		~Skybox() = default;

		virtual void Draw(shared_ptr<OpenGLRendererAPI> api) override;
	};
}