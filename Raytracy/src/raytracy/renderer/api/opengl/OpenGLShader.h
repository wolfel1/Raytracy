#pragma once

#include "OpenGLBuffer.h"

typedef unsigned int GLenum;

namespace raytracy {
	class OpenGLShader {
	protected:
		std::string name = "";
		uint32_t renderer_id{};
		static uint32_t uniform_buffer_index;

	public:
		OpenGLShader(const std::string& name);
		OpenGLShader(const std::vector<std::string>& paths);
		~OpenGLShader();

		const std::string& GetName() const { return name; }

		void Bind() const;
		void Unbind() const;
		static shared_ptr<OpenGLShader> CreateFromFile(const std::string& name);
		static shared_ptr<OpenGLShader> CreateFromDirectory(const std::string& directory_name);
		BufferLayout const GetUniformBufferLayout(UniformBlock const& uniform_block) const;

		void AddUniformBuffer(shared_ptr<OpenGLUniformBuffer> const uniform_buffer);
		void BindBuffer(shared_ptr<OpenGLUniformBuffer> const uniform_buffer);

		void SetInt(std::string const& name, uint32_t value);
		void SetVec3(std::string const& name, glm::vec3 const& value);
	private:

		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void PreProcess(const std::string& path, std::unordered_map<GLenum, std::string>& shaderSources);
		void Compile(std::unordered_map<GLenum, std::string> shaderSources);

		void BindBuffer(std::string const& name);
		void CreateCameraUniformBuffer();
		void CreateLightUniformBuffer();
	};
}