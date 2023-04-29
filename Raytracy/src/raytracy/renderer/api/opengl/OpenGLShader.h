#pragma once

#include "../Shader.h"

typedef unsigned int GLenum;

namespace raytracy {
	class OpenGLShaderProgram : public ShaderProgram {

	public:
		OpenGLShaderProgram(const std::string& name);
		OpenGLShaderProgram(const std::vector<std::string>& paths);
		~OpenGLShaderProgram();

		virtual void Bind() const override;

		virtual void Unbind() const override;

		static shared_ptr<OpenGLShaderProgram> CreateFromFile(const std::string& name);

		static shared_ptr<OpenGLShaderProgram> CreateFromDirectory(const std::string& directory_name);

	private:

		std::string ReadFile(const std::string& path);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void PreProcess(const std::string& path, std::unordered_map<GLenum, std::string>& shaderSources);
		void Compile(std::unordered_map<GLenum, std::string> shaderSources);
	};
}