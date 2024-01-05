#pragma once

#include "OpenGLBuffer.h"

typedef unsigned int GLenum;

namespace raytracy {
	class OpenGLShader {
	protected:
		std::string name = "";
		uint32_t renderer_id{};
		static uint32_t index;
		shared_ptr<OpenGLUniformBuffer> material_uniform_buffer;



	public:
		OpenGLShader(const std::string& name);
		OpenGLShader(const std::vector<std::string>& paths);
		~OpenGLShader();

		const std::string& GetName() const { return name; }

		 void Bind() const;
		 void Unbind() const;
		 static shared_ptr<OpenGLShader> CreateFromFile(const std::string& name);
		 static shared_ptr<OpenGLShader> CreateFromDirectory(const std::string& directory_name);

		 shared_ptr<OpenGLUniformBuffer> GetMaterialUniformBuffer() const {
			 return material_uniform_buffer;
		 }
		 void AddUniformBuffer(std::string const& name, shared_ptr<OpenGLUniformBuffer> const uniform_buffer);
	private:

		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void PreProcess(const std::string& path, std::unordered_map<GLenum, std::string>& shaderSources);
		void Compile(std::unordered_map<GLenum, std::string> shaderSources);

		BufferLayout const GetUniformBufferLayout(UniformBlock const& uniform_block) const;
		void BindBuffer(shared_ptr<OpenGLUniformBuffer> const uniform_buffer) ;
		void CreateCameraUniformBuffer();
		void CreateLightUniformBuffer();
		void CreateMaterialUniformBuffer();
	};
}