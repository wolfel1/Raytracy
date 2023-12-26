#pragma once

#include "../Shader.h"

typedef unsigned int GLenum;

namespace raytracy {
	class OpenGLShader : public Shader {

	public:
		OpenGLShader(const std::string& name);
		OpenGLShader(const std::vector<std::string>& paths);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual BufferLayout const GetUniformBufferLayout(UniformBlock const& uniform_block) const override;
		void CreateCameraUniformBuffer();
		void CreateLightUniformBuffer();

		static shared_ptr<OpenGLShader> CreateFromFile(const std::string& name);

		static shared_ptr<OpenGLShader> CreateFromDirectory(const std::string& directory_name);

	private:

		std::string ReadFile(const std::string& path);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void PreProcess(const std::string& path, std::unordered_map<GLenum, std::string>& shaderSources);
		void Compile(std::unordered_map<GLenum, std::string> shaderSources);

		virtual void BindBuffer(shared_ptr<UniformBuffer> const uniform_buffer) override;
	};
}