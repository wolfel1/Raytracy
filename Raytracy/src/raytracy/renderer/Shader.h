#pragma once

typedef unsigned int GLenum;

namespace raytracy {
	class ShaderProgram {
	private:
		const std::string rootPath = "resources/shaders/";

		std::string name = "";
		uint32_t renderer_id;

	public:

		ShaderProgram(const std::string& name);
		ShaderProgram(const std::vector<std::string>& paths);
		~ShaderProgram();

		void Bind() const;

		void Unbind() const;

	private:
		std::string ReadFile(const std::string& path);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void PreProcess(const std::string& path, std::unordered_map<GLenum, std::string>& shaderSources);
		void Compile(std::unordered_map<GLenum, std::string> shaderSources);
	};
}