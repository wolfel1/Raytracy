#pragma once

#include <thread>

#include "../camera/PerspectiveCamera.h"
#include "../api/opengl/OpenGLTexture.h"
#include "../api/opengl/OpenGLRendererAPI.h"
#include "../api/opengl/OpenGLShader.h"
#include "../ViewportScene.h"
#include <glm/glm.hpp>

namespace raytracy {

	struct Node {
		glm::vec3 min_corner;
		uint32_t left_child_index;
		glm::vec3 max_corner;
		uint32_t right_child_index;
		alignas(16) bool has_triangle;
		uint32_t lookup_index;
		uint32_t triangle_count;
	};

	struct RTriangle {
		std::array<uint32_t, 3> vertex_indices;
	};

	struct RVertex {
		alignas(16) glm::vec3 position;
		alignas(16) glm::vec3 normal;
		glm::vec4 color;
		glm::vec2 tex_coords;
	};

	class Raytracer {
	private:

		shared_ptr<OpenGLRendererAPI> renderer_api = nullptr;

		shared_ptr<OpenGLTexture2D> raytracing_canvas = nullptr;

		shared_ptr<OpenGLShader> raytracing_kernel;
		shared_ptr<OpenGLShader> canvas_shader;

		shared_ptr<OpenGLUniformBuffer> scene_data_uniform_buffer;
		shared_ptr<OpenGLStorageBuffer> triangles_storage_buffer;
		shared_ptr<OpenGLStorageBuffer> triangle_indices_storage_buffer;
		shared_ptr<OpenGLStorageBuffer> vertices_storage_buffer;
		shared_ptr<OpenGLStorageBuffer> bvh_storage_buffer;

		glm::uvec2 screen_size{};
		uint32_t canvas_size = 2048;

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