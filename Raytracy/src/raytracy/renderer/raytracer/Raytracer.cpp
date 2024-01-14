#include "raytracypch.h"
#include "Raytracer.h"

#include <execution>

#include "../../Application.h"

#include <glad/gl.h>

namespace raytracy {

	GLuint scene_buffer;


	void Raytracer::Init(shared_ptr<OpenGLRendererAPI> const api) {
		renderer_api = api;

		auto& app_spec = Application::Get().GetSpecification();
		raytracing_canvas = OpenGLTexture2D::Create(app_spec.width, app_spec.height, GL_RGBA32F);
		raytracing_kernel = ShaderLibrary::Get().Load("raytrace_kernel");
		raytracing_output = ShaderLibrary::Get().Load("raytrace_output");

		scene_storage_buffer = OpenGLStorageBuffer::Create("Scene", 8192);
		scene_storage_buffer->Bind();
		scene_storage_buffer->BindSlot(0);
		glCreateBuffers(1, &scene_buffer);

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

		scene_data_uniform_buffer->SetInt("samples", 10);
		scene_data_uniform_buffer->SetInt("max_depth", 50);

	}

	void Raytracer::Raytrace(shared_ptr<renderer::Scene> const scene) {
		Preprocess(scene);

		renderer_api->ClearViewport();

		raytracing_kernel->Bind();
		raytracing_canvas->BindImage();
		renderer_api->LaunchComputeShader(raytracing_canvas->GetWidth(), raytracing_canvas->GetHeight(), 1);

		renderer_api->SetMemoryBarrier();

		raytracing_canvas->Bind(0);
		raytracing_output->Bind();
		raytracing_output->SetInt("tex", 0);
		renderer_api->Draw(4);
	}


	void Raytracer::Preprocess(shared_ptr<renderer::Scene> const scene) {
		std::vector<Sphere> spheres;
		for (auto mesh : scene->GetMeshes()) {
			spheres.push_back({mesh->GetMaterial()->GetColor(), mesh->GetOrigin(), mesh->GetScale()});
		}
		scene_storage_buffer->SetData(sizeof(Sphere) * spheres.size(), spheres.data());

		auto camera = scene->GetCamera();
		scene_data_uniform_buffer->SetMat4("inverse_view", glm::inverse(camera->GetViewMatrix()));
		scene_data_uniform_buffer->SetMat4("inverse_projection", glm::inverse(camera->GetProjectionMatrix()));
		scene_data_uniform_buffer->SetVec3("camera_position", camera->GetPosition());
		scene_data_uniform_buffer->SetVec3("camera_direction", camera->GetDirection());
	}

	void Raytracer::Shutdown() {
		renderer_api = nullptr;
		scene_data_uniform_buffer = nullptr;
	}

}  // namespace raytracy
