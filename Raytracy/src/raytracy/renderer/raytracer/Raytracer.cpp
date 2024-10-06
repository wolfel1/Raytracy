#include "raytracypch.h"
#include "Raytracer.h"

#include <execution>

#include "../../Application.h"
#include "../model/Material.h"

#include <glad/gl.h>

namespace raytracy {


	void Raytracer::Init(shared_ptr<OpenGLRendererAPI> const api) {
		renderer_api = api;

		auto app_spec = IApplication::Get()->GetSpecification();
		raytracing_canvas = OpenGLTexture2D::Create(app_spec.width, app_spec.height, GL_RGBA32F);
		raytracing_kernel = ShaderLibrary::Get().Load("raytrace_kernel");
		raytracing_output = ShaderLibrary::Get().Load("raytrace_output");

		triangles_storage_buffer = OpenGLStorageBuffer::Create("Triangles");
		triangles_storage_buffer->Bind();
		triangles_storage_buffer->BindSlot(0);

		vertices_storage_buffer = OpenGLStorageBuffer::Create("Vertices");
		vertices_storage_buffer->Bind();
		vertices_storage_buffer->BindSlot(1);

		bvh_storage_buffer = OpenGLStorageBuffer::Create("BoundingVolumeHierarchie");
		bvh_storage_buffer->Bind();
		bvh_storage_buffer->BindSlot(2);

		triangle_indices_storage_buffer = OpenGLStorageBuffer::Create("TriangleIndices");
		triangle_indices_storage_buffer->Bind();
		triangle_indices_storage_buffer->BindSlot(3);

		UniformBlock block("SceneData", {
			"inverse_view",
			"inverse_projection",
			"camera_position",
			"samples",
			"camera_direction",
			"max_depth"
		});

		auto layout = raytracing_kernel->GetUniformBufferLayout(block);

		scene_data_uniform_buffer = OpenGLUniformBuffer::Create("SceneData", layout);
		raytracing_kernel->AddUniformBuffer(scene_data_uniform_buffer);

		scene_data_uniform_buffer->SetInt("samples", 5);
		scene_data_uniform_buffer->SetInt("max_depth", 50);

	}

	void Raytracer::Raytrace(shared_ptr<renderer::Scene> const scene) {
		Preprocess(scene);

		renderer_api->ClearViewport();

		raytracing_kernel->Bind();
		raytracing_kernel->SetInt("skybox", 1);
		raytracing_canvas->BindImage();
		scene_data_uniform_buffer->Bind();
		renderer_api->LaunchComputeShader(raytracing_canvas->GetWidth(), raytracing_canvas->GetHeight(), 1);

		renderer_api->SetMemoryBarrier();

		raytracing_canvas->Bind(0);
		raytracing_output->Bind();
		raytracing_output->SetInt("tex", 0);
		renderer_api->Draw(4);
	}

	void Raytracer::Preprocess(shared_ptr<renderer::Scene> const scene) {
#if !TRIANGLES
		std::vector<RTriangle> triangles;
		std::vector<RVertex> vertices;

		for (auto triangle_ptr : scene->GetTriangles()) {
			RTriangle triangle{};
			triangle.center = triangle_ptr->center;

			auto& corners = triangle_ptr->vertices;
			for (uint32_t i = 0; i < 3; i++) {
				triangle.vertex_indices[i] = vertices.size() + i;
				vertices.push_back({ corners[i]->position, corners[i]->normal, corners[i]->color, corners[i]->tex_coords });
			}
			triangles.push_back(triangle);
		}
		triangles_storage_buffer->SetData(sizeof(RTriangle) * triangles.size(), triangles.data());
		vertices_storage_buffer->SetData(sizeof(RVertex) * vertices.size(), vertices.data());

		std::vector<TriangleNode> bounding_volume_hierarchie;
		std::vector<uint32_t> triangle_indices;
		uint32_t lookup_index = 0;
		for (auto& bvh_node : scene->GetBoundingVolumeHierarchie()) {
			TriangleNode node{};
			node.left_child_index = bvh_node.left_child_index;
			node.right_child_index = bvh_node.right_child_index;
			node.min_corner = bvh_node.min_corner;
			node.max_corner = bvh_node.max_corner;
			if (node.has_object = !bvh_node.object_indices.empty()) {
				node.model_matrix = bvh_node.model_matrix;
				node.triangle_count = bvh_node.triangle_indices.size();
				triangle_indices.insert(std::end(triangle_indices), std::begin(bvh_node.triangle_indices), std::end(bvh_node.triangle_indices));
				node.lookup_index = lookup_index;
				lookup_index = triangle_indices.size();
			}
			bounding_volume_hierarchie.push_back(node);
		}

		triangle_indices_storage_buffer->SetData(sizeof(uint32_t) * triangle_indices.size(), triangle_indices.data());
#else
		std::vector<Sphere> spheres;
		for (auto mesh : scene->GetMeshes()) {
			auto color = dynamic_pointer_cast<renderer::MeshMaterial>(mesh->GetMaterial())->GetColor();
			spheres.push_back({ color, mesh->GetOrigin(), mesh->GetScale() });
		}
		scene_storage_buffer->SetData(sizeof(Sphere) * spheres.size(), spheres.data());

		std::vector<Node> bounding_volume_hierarchie;
		for (auto& bvh_node : scene->GetBoundingVolumeHierarchie()) {
			Node node{};
			node.left_child_index = bvh_node.left_child_index;
			node.right_child_index = bvh_node.right_child_index;
			node.min_corner = bvh_node.min_corner;
			node.max_corner = bvh_node.max_corner;
			if (node.has_object = !bvh_node.object_indices.empty()) {
				node.object_index = bvh_node.object_indices[0];
			}
			bounding_volume_hierarchie.push_back(node);
		}


#endif
		bvh_storage_buffer->SetData(sizeof(Node) * bounding_volume_hierarchie.size(), bounding_volume_hierarchie.data());

		auto camera = scene->GetCamera();
		scene_data_uniform_buffer->SetMat4("inverse_view", glm::inverse(camera->GetViewMatrix()));
		scene_data_uniform_buffer->SetMat4("inverse_projection", glm::inverse(camera->GetProjectionMatrix()));
		scene_data_uniform_buffer->SetVec3("camera_position", camera->GetPosition());
		scene_data_uniform_buffer->SetVec3("camera_direction", camera->GetDirection());
	}

	void Raytracer::Shutdown() {
		renderer_api = nullptr;
		scene_data_uniform_buffer = nullptr;
		scene_storage_buffer = nullptr;
		bvh_storage_buffer = nullptr;
	}

	bool Raytracer::OnWindowResize(uint32_t width, uint32_t height) {
		raytracing_canvas = OpenGLTexture2D::Create(width, height, GL_RGBA32F);
		return true;
	}

}  // namespace raytracy
