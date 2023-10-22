#include "raytracypch.h"
#include "Shader.h"

#include "RendererAPI.h"
#include "opengl/OpenGLShader.h"
#include <filesystem>

namespace raytracy {

	const std::string ShaderLibrary::rootPath = "resources/shaders/";

	shared_ptr<Shader> Shader::CreateFromFile(const std::string& name) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:
			RTY_ASSERT(false, "Renderer API None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return make_shared<OpenGLShader>(name);
		default:
			break;
		}

		RTY_ASSERT(false, "Unknown RendereAPI!");
		return nullptr;
	}

	shared_ptr<Shader> Shader::CreateFromDirectory(const std::string& directory_name) {
		std::string path = ShaderLibrary::rootPath + directory_name;
		std::vector<std::string> filepaths;
		for (const auto& file : std::filesystem::directory_iterator(path)) {
			filepaths.push_back(file.path().string());
		}


		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:
			RTY_ASSERT(false, "Renderer API None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return make_shared<OpenGLShader>(filepaths);
		default:
			break;
		}

		RTY_ASSERT(false, "Unknown RendereAPI!");
		return nullptr;

	}



	shared_ptr<Shader> ShaderLibrary::Load(const std::string& name) {
		std::string path = rootPath + name;
		shared_ptr<Shader> shader = nullptr;
		if (Exist(name)) {
			return shader_programs[name];
		}
		if (std::filesystem::is_directory(path)) {
			shader = Shader::CreateFromDirectory(name);
		} else {
			shader = Shader::CreateFromFile(name);
		}
		RTY_ASSERT(shader, "Could not create shader {0}!", name);
		Add(shader);
		return shader;
	}

	void ShaderLibrary::Add(const shared_ptr<Shader>& shader_program) {
		shader_programs[shader_program->GetName()] = shader_program;
		RTY_RENDERER_TRACE("{0} shader successfully added!", shader_program->GetName());
	}

	void ShaderLibrary::Remove(const std::string& name) {
		if (Exist(name)) {
			shader_programs.erase(name);
		}
		RTY_RENDERER_ERROR("Could not remove shader program refered as {0}!", name);
	}
	bool ShaderLibrary::Exist(const std::string& name) {
		return shader_programs.find(name) != shader_programs.end();
	}
}