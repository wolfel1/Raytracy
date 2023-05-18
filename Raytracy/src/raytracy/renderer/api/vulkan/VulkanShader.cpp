#include "raytracypch.h"
#include "VulkanShader.h"

#include <filesystem>

namespace raytracy {

	const std::string ShaderLibrary::rootPath = "resources/shaders/";

	shared_ptr<VulkanShader> VulkanShader::CreateFromFile(const std::string& name) {
			return make_shared<VulkanShader>(name);
	}

	shared_ptr<VulkanShader> VulkanShader::CreateFromDirectory(const std::string& directory_name) {
		std::string path = ShaderLibrary::rootPath + directory_name;
		std::vector<std::string> filepaths;
		for (const auto& file : std::filesystem::directory_iterator(path)) {
			filepaths.push_back(file.path().string());
		}

		return make_shared<VulkanShader>(filepaths);
	}

	VulkanShader::VulkanShader(const std::string& name) {
	}
	VulkanShader::VulkanShader(const std::vector<std::string>& paths) {
	}
	VulkanShader::~VulkanShader() {
	}
	void VulkanShader::Bind() const {
	}
	void VulkanShader::Unbind() const {
	}

	shared_ptr<VulkanShader> ShaderLibrary::Load(const std::string& name) {
		std::string path = rootPath + name;
		shared_ptr<VulkanShader> shader = nullptr;
		if (Exist(name)) {
			return shader_programs[name];
		}
		if (std::filesystem::is_directory(path)) {
			shader = VulkanShader::CreateFromDirectory(name);
		} else {
			shader = VulkanShader::CreateFromFile(name);
		}
		RTY_ASSERT(shader, "Could not create shader {0}!", name);
		Add(shader);
		return shader;
	}

	void ShaderLibrary::Add(const shared_ptr<VulkanShader>& shader_program) {
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