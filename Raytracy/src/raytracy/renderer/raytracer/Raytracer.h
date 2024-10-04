#pragma once

#include <thread>

#include "../camera/PerspectiveCamera.h"
#include "../api/opengl/OpenGLTexture.h"
#include "../api/opengl/OpenGLRendererAPI.h"
#include "../api/opengl/OpenGLShader.h"
#include "../ViewportScene.h"
#include <glm/glm.hpp>

namespace raytracy {

	struct Sphere{
		glm::vec4 color;
		glm::vec3 origin;
		float radius;
	};

	struct Node {
		glm::vec3 min_corner;
		uint32_t left_child_index;
		glm::vec3 max_corner;
		uint32_t right_child_index;
		alignas(16) uint32_t object_index;
		bool has_object;
	};

	struct TriangleNode {
		glm::vec3 min_corner;
		uint32_t left_child_index;
		glm::vec3 max_corner;
		uint32_t right_child_index;
		glm::mat4 model_matrix;
		bool has_object;
		std::vector<uint32_t> triangle_indices;
	};

	struct RTriangle {
		glm::vec3 center;
		std::array<uint32_t, 3> vertex_indices;
	};

	struct RVertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec4 color;
		glm::vec2 tex_coords;
	};

	class Raytracer {
	private:

		shared_ptr<OpenGLRendererAPI> renderer_api = nullptr;

		shared_ptr<OpenGLTexture2D> raytracing_canvas = nullptr;

		shared_ptr<OpenGLShader> raytracing_kernel;
		shared_ptr<OpenGLShader> raytracing_output;

		shared_ptr<OpenGLUniformBuffer> scene_data_uniform_buffer;
		shared_ptr<OpenGLStorageBuffer> scene_storage_buffer;
		shared_ptr<OpenGLStorageBuffer> triangles_storage_buffer;
		shared_ptr<OpenGLStorageBuffer> vertices_storage_buffer;
		shared_ptr<OpenGLStorageBuffer> bvh_storage_buffer;

	public:
		Raytracer(const Raytracer&) = delete;
		~Raytracer() = default;

		static Raytracer& Get() {
			static Raytracer raytracer;
			return raytracer;
		}
		void Init(shared_ptr<OpenGLRendererAPI> const renderer_api);
		void Raytrace(shared_ptr<renderer::Scene> const scene);
		bool OnWindowResize(uint32_t width, uint32_t height);

		void Shutdown();

	private:
		Raytracer() {}

		void Preprocess(shared_ptr<renderer::Scene> const scene);
	};
}