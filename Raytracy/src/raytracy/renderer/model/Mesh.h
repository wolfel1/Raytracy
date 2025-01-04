#pragma once

#include "../api/opengl/OpenGLBuffer.h"
#include "../api/opengl/OpenGLVertexArray.h"
#include "../api/opengl/OpenGLRendererAPI.h"
#include "Primitives.h"
#include "Material.h"
#include "../raytracer/BoundingBoxNode.h"

namespace raytracy::renderer {

	

	class Mesh {
	protected:
		static shared_ptr<Material> default_material;
		shared_ptr<OpenGLVertexArray> vertex_array;
		shared_ptr<Material> material;

		bool is_indexed = false;

		glm::mat4 model_matrix = glm::mat4(1.0f);
		glm::vec3 origin{};
		float scale = 1.0f;

		std::vector<shared_ptr<Triangle>> triangles;

		BoundingBox bounding_box;

	public:
		Mesh() {}
		Mesh(shared_ptr<MeshData> const mesh_data, glm::vec3 const& position, float const scale_factor);
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

		glm::vec3 const& GetOrigin() const {
			return origin;
		}

		BoundingBox const& GetBoundingBox() const {
			return bounding_box;
		}

		float GetScale() const {
			return scale;
		}

		std::vector<shared_ptr<Triangle>> const& GetTriangles() const {
			return triangles;
		}

		virtual void Draw(shared_ptr<OpenGLRendererAPI> api);

		void Translate(glm::vec3 const& direction);

		void Rotate(glm::vec3 const& axis, float const value);

		void Scale(float const value);

	protected:
		void Init(shared_ptr<MeshData> const mesh_data);

	private:
		void AddDefaultMaterial();
		void BuildTriangles(shared_ptr<MeshData> const mesh_data);
		void BuildBoundingBox(shared_ptr<MeshData> const mesh_data);
		void UpdateBoundingBox(glm::mat4 const& transformation_matrix);
	};

	class Plane : public Mesh {

	public:
		Plane(glm::vec3 const& position = {}, float const scale_factor = 1.0f);
		~Plane() = default;
	};

	class Cube : public Mesh {

	public:
		Cube(glm::vec3 const position = {}, float const scale_factor = 1.0f);
		~Cube() = default;
	};

	class Sphere : public Mesh {
	public:
		Sphere(glm::vec3 const position = {}, float const scale_factor = 1.0f);
		~Sphere() = default;
	};

	class Skybox : public Mesh {

	public:
		Skybox();
		~Skybox() = default;

		virtual void Draw(shared_ptr<OpenGLRendererAPI> api) override;
	};
}