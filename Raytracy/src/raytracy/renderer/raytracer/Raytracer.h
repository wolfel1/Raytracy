#pragma once

#include <thread>

#include "../camera/PerspectiveCamera.h"
#include "../api/opengl/OpenGLTexture.h"
#include "../api/opengl/OpenGLRendererAPI.h"
#include "../api/opengl/OpenGLShader.h"
#include "../ViewportScene.h"
#include <glm/glm.hpp>
#include <taskflow.hpp>

namespace raytracy {

	

	class Raytracer {

		struct Node {
			alignas(16) glm::vec3 min_corner;
			uint32_t left_child_index;
			alignas(16) glm::vec3 max_corner;
			uint32_t right_child_index;
			alignas(16) bool has_triangle;
			uint32_t lookup_index;
			uint32_t triangle_count;
		};

		struct Mesh {
			uint32_t material_index;
		};

		struct Triangle {
			alignas(16) std::array<uint32_t, 3> vertex_indices;
			uint32_t mesh_index;
		};

		struct Vertex {
			alignas(16) glm::vec3 position;
			alignas(16) glm::vec3 normal;
			alignas(16) glm::vec4 color;
			alignas(16) glm::vec2 tex_coords;
		};

		struct DirectionalLight {
			alignas(16) glm::vec3 color;
			float strength;
			alignas(16) glm::vec3 direction;
		};

		struct Material {
			alignas(16) glm::vec4 color;
			alignas(16) glm::vec3 specular;
			float shininess;
		};

	private:

		tf::Executor executor;

		shared_ptr<OpenGLRendererAPI> renderer_api = nullptr;

		shared_ptr<OpenGLTexture2D> raytracing_canvas = nullptr;

		shared_ptr<OpenGLShader> raytracing_kernel;
		shared_ptr<OpenGLShader> canvas_shader;

		shared_ptr<OpenGLUniformBuffer> scene_data_uniform_buffer;
		shared_ptr<OpenGLStorageBuffer> meshes_storage_buffer;
		shared_ptr<OpenGLStorageBuffer> triangles_storage_buffer;
		shared_ptr<OpenGLStorageBuffer> triangle_indices_storage_buffer;
		shared_ptr<OpenGLStorageBuffer> vertices_storage_buffer;
		shared_ptr<OpenGLStorageBuffer> light_storage_buffer;
		shared_ptr<OpenGLStorageBuffer> material_storage_buffer;
		shared_ptr<OpenGLStorageBuffer> bvh_storage_buffer;

		glm::uvec2 screen_size{};
		uint32_t canvas_size = 2048;
		uint32_t work_group_size = 16;

	public:
		Raytracer(const Raytracer&) = delete;
		~Raytracer() = default;

		static Raytracer& Get() {
			static Raytracer raytracer;
			return raytracer;
		}
		void Init(shared_ptr<OpenGLRendererAPI> const renderer_api);
		void Raytrace(shared_ptr<renderer::Scene> const scene);

		void Shutdown();

	private:
		Raytracer() {}

		void Preprocess(shared_ptr<renderer::Scene> const scene);
	};
}