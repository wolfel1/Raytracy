#include "raytracypch.h"
#include "Shader.h"

#include "opengl/OpenGLShader.h"
#include <filesystem>

namespace raytracy {

	const std::string ShaderLibrary::rootPath = "resources/shaders/";

	shared_ptr<ShaderProgram> ShaderProgram::CreateFromFile(const std::string& name) {
		return make_shared<OpenGLShaderProgram>(name);
	}

	shared_ptr<ShaderProgram> ShaderProgram::CreateFromDirectory(const std::string& directory_name) {
		std::string path = ShaderLibrary::rootPath + directory_name;
		std::vector<std::string> filepaths;
		for (const auto& file : std::filesystem::directory_iterator(path)) {
			filepaths.push_back(file.path().string());
		}

		return make_shared<OpenGLShaderProgram>(filepaths);

	}

	shared_ptr<ShaderProgram> ShaderLibrary::Load(const std::string& name) {
		std::string path = rootPath + name;
		shared_ptr<ShaderProgram> shader = nullptr;
		if (Exist(name)) {
			return shader_programs[name];
		}
		if (std::filesystem::is_directory(path)) {
			shader = ShaderProgram::CreateFromDirectory(name);
		} else {
			shader = ShaderProgram::CreateFromFile(name);
		}
		RTY_ASSERT(shader, "Could not create shader {0}!", name);
		Add(shader);
		return shader;
	}

	void ShaderLibrary::Add(const shared_ptr<ShaderProgram>& shader_program) {
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