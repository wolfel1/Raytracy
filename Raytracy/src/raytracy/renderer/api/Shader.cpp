#include "raytracypch.h"
#include "Shader.h"

#include "opengl/OpenGLShader.h"
#include <filesystem>

namespace raytracy {

	const std::string ShaderLibrary::rootPath = "resources/shaders/";


	shared_ptr<OpenGLShader> ShaderLibrary::Load(const std::string& name) {
		std::string path = rootPath + name;
		shared_ptr<OpenGLShader> shader = nullptr;
		if (Exist(name)) {
			return shaders[name];
		}
		if (std::filesystem::is_directory(path)) {
			shader = OpenGLShader::CreateFromDirectory(name);
		} else {
			shader = OpenGLShader::CreateFromFile(name);
		}
		RTY_ASSERT(shader, "Could not create shader {0}!", name);
		Add(shader);
		return shader;
	}

	void ShaderLibrary::Add(const shared_ptr<OpenGLShader> shader_program) {
		shaders[shader_program->GetName()] = shader_program;
		RTY_RENDERER_TRACE("{0} shader successfully added!", shader_program->GetName());
	}

	void ShaderLibrary::Remove(const std::string& name) {
		if (Exist(name)) {
			shaders.erase(name);
		}
		RTY_RENDERER_ERROR("Could not remove shader program refered as {0}!", name);
	}
	bool ShaderLibrary::Exist(const std::string& name) {
		return shaders.find(name) != shaders.end();
	}
}