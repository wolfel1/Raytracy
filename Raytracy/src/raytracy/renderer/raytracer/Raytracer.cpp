#include "raytracypch.h"
#include "Raytracer.h"

#include <execution>

#include "../../Application.h"
#include "../model/Material.h"

#include <glad/gl.h>
#include <taskflow/algorithm/for_each.hpp>

namespace raytracy {

	void Raytracer::Init(shared_ptr<OpenGLRendererAPI> const api) {
		RTY_PROFILE_FUNCTION();
		renderer_api = api;

		auto& app_spec = IApplication::Get()->GetSpecification();
		screen_size = { app_spec.width, app_spec.height };
		raytracing_canvas = OpenGLTexture2D::Create(canvas_size, canvas_size, GL_RGBA16F);
		raytracing_kernel = ShaderLibrary::Get().Load("raytracing");
		canvas_shader = ShaderLibrary::Get().Load("canvas");

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

		light_storage_buffer = OpenGLStorageBuffer::Create("Lights");
		light_storage_buffer->Bind();
		light_storage_buffer->BindSlot(4);

		material_storage_buffer = OpenGLStorageBuffer::Create("Materials");
		material_storage_buffer->Bind();
		material_storage_buffer->BindSlot(5);

		meshes_storage_buffer = OpenGLStorageBuffer::Create("Meshes");
		meshes_storage_buffer->Bind();
		meshes_storage_buffer->BindSlot(6);

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

		scene_data_uniform_buffer->SetInt("samples", 1);
		scene_data_uniform_buffer->SetInt("max_depth", 4);

	}

	void Raytracer::Raytrace(shared_ptr<renderer::Scene> const scene) {
		RTY_PROFILE_FUNCTION();
		Preprocess(scene);

		renderer_api->ClearViewport();

		raytracing_kernel->Bind();
		raytracing_kernel->SetInt("skybox", 1);
		raytracing_canvas->BindImage();
		scene_data_uniform_buffer->Bind();
		renderer_api->LaunchComputeShader(static_cast<uint32_t>(canvas_size / work_group_size), static_cast<uint32_t>(canvas_size / work_group_size), 1);

		renderer_api->SetMemoryBarrier();

		raytracing_canvas->Bind(0);
		canvas_shader->Bind();
		canvas_shader->SetInt("tex", 0);
		renderer_api->Draw(4);
	}

	void Raytracer::Preprocess(shared_ptr<renderer::Scene> const scene) {
		RTY_PROFILE_FUNCTION();

		std::vector<DirectionalLight> lights_data;
		auto lights_task = taskflow.emplace([&]() {
			{
				RTY_PROFILE_SCOPE("Lights");
				lights_data.reserve(1);
				auto light = scene->GetLight();
				if (light) {
					DirectionalLight& dir_light = lights_data.emplace_back();
					dir_light.color = light->GetColor();
					dir_light.direction = light->GetDirection();
					dir_light.strength = light->GetStrength();
				}
			}
		});

		std::vector<Material> materials_data;
		std::vector<Mesh> meshes_data;

		auto material_task = taskflow.emplace([&]() {
			{
				RTY_PROFILE_SCOPE("Materials");
				auto& materials = renderer::MaterialLibrary::Get().GetMaterials();
				materials_data.reserve(materials.size());
				auto& meshes = scene->GetMeshes();
				meshes_data.reserve(meshes.size());
				for (auto const& [name, material] : materials) {
					Material& mat = materials_data.emplace_back();
					mat.color = material->GetColor();
					mat.specular = material->GetSpecular();
					mat.shininess = material->GetShininess();
				};

				for (auto const& mesh : meshes) {
					Mesh& mesh_data = meshes_data.emplace_back();
					auto material = mesh->GetMaterial();
					mesh_data.material_index = static_cast<uint32_t>(std::distance(std::begin(materials), materials.find(material->GetName())));
				}
			}
		});


		std::vector<Node> bounding_volume_hierarchie;
		std::vector<uint32_t> triangle_indices;

		auto bvh_task = taskflow.emplace([&]() {
			{
				RTY_PROFILE_SCOPE("BVH");
				auto& scene_bvh = scene->GetBoundingVolumeHierarchie();
				bounding_volume_hierarchie.resize(scene_bvh.size());
				triangle_indices.reserve(scene->GetTriangles().size());
				std::atomic<uint32_t> bvh_index = 0;
				uint32_t lookup_index = 0;
				for (auto const& bvh_node : scene_bvh) {
					Node& node = bounding_volume_hierarchie[bvh_index++];
					node.left_child_index = bvh_node.left_child_index;
					node.right_child_index = bvh_node.right_child_index;
					node.min_corner = bvh_node.min_corner;
					node.max_corner = bvh_node.max_corner;
					if (node.has_triangle = !bvh_node.triangle_indices.empty()) {
						node.triangle_count = static_cast<uint32_t>(bvh_node.triangle_indices.size());
						node.lookup_index = lookup_index;
						triangle_indices.insert(std::end(triangle_indices), std::begin(bvh_node.triangle_indices), std::end(bvh_node.triangle_indices));
						lookup_index = triangle_indices.size();
					}
				}
			}
		});
		auto future = executor.run(taskflow);

		std::vector<Triangle> triangles;
		std::vector<Vertex> vertices;
		std::atomic<uint32_t> triangle_index = 0;
		std::atomic<uint32_t> vertex_index = 0;

		auto& triangles_data = scene->GetTriangles();
		triangles.resize(triangles_data.size());
		vertices.resize(triangles_data.size() * 3);
		{
			RTY_PROFILE_SCOPE("Triangles");
			for (auto const& triangle_data : triangles_data) {
				Triangle& triangle = triangles[triangle_index++];
				triangle.mesh_index = triangle_data.mesh_index;
				auto& corners = triangle_data.vertices;
				for (uint32_t i = 0; i < 3; i++) {
					uint32_t local_vertex_index = vertex_index++;
					triangle.vertex_indices[i] = local_vertex_index;
					Vertex& vertex = vertices[local_vertex_index];
					vertex.position = corners[i].position;
					vertex.normal = corners[i].normal;
					vertex.color = corners[i].color;
					vertex.tex_coords = corners[i].tex_coords;
				}
			}
		}

		auto camera = scene->GetCamera();
		scene_data_uniform_buffer->SetMat4("inverse_view", glm::inverse(camera->GetViewMatrix()));
		scene_data_uniform_buffer->SetMat4("inverse_projection", glm::inverse(camera->GetProjectionMatrix()));
		scene_data_uniform_buffer->SetVec3("camera_position", camera->GetPosition());
		scene_data_uniform_buffer->SetVec3("camera_direction", camera->GetDirection());

		future.wait();
		triangle_indices_storage_buffer->SetData(sizeof(uint32_t) * triangle_indices.size(), triangle_indices.data());
		bvh_storage_buffer->SetData(sizeof(Node) * bounding_volume_hierarchie.size(), bounding_volume_hierarchie.data());
		triangles_storage_buffer->SetData(sizeof(Triangle) * triangles.size(), triangles.data());
		vertices_storage_buffer->SetData(sizeof(Vertex) * vertices.size(), vertices.data());
		meshes_storage_buffer->SetData(sizeof(Mesh) * meshes_data.size(), meshes_data.data());
		material_storage_buffer->SetData(sizeof(Material) * materials_data.size(), materials_data.data());
		light_storage_buffer->SetData(sizeof(DirectionalLight) * lights_data.size(), lights_data.data());

		taskflow.clear();
	}

	void Raytracer::Shutdown() {
		RTY_PROFILE_FUNCTION();
		renderer_api = nullptr;
		scene_data_uniform_buffer = nullptr;
		bvh_storage_buffer = nullptr;
		triangles_storage_buffer = nullptr;
		vertices_storage_buffer = nullptr;
		triangle_indices_storage_buffer = nullptr;
		light_storage_buffer = nullptr;
		material_storage_buffer = nullptr;
		meshes_storage_buffer = nullptr;
	}
}  // namespace raytracy
