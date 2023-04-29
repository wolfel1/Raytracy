#pragma once

typedef unsigned int GLenum;

namespace raytracy {
	class OpenGLShaderProgram {
	private:
		std::string name = "";
		uint32_t renderer_id;

	public:

		OpenGLShaderProgram(const std::string& name);
		OpenGLShaderProgram(const std::vector<std::string>& paths);
		~OpenGLShaderProgram();

		const std::string& GetName() const { return name; }

		void Bind() const;

		void Unbind() const;

		static shared_ptr<OpenGLShaderProgram> CreateFromFile(const std::string& name);

		static shared_ptr<OpenGLShaderProgram> CreateFromDirectory(const std::string& directory_name);

	private:

		std::string ReadFile(const std::string& path);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void PreProcess(const std::string& path, std::unordered_map<GLenum, std::string>& shaderSources);
		void Compile(std::unordered_map<GLenum, std::string> shaderSources);
	};

	class ShaderLibrary {
	private:
		std::unordered_map<std::string, shared_ptr<OpenGLShaderProgram>> shader_programs;

	public:
		static const std::string rootPath;

	public:
		ShaderLibrary(const ShaderLibrary&) = delete;

		static ShaderLibrary& Get() {
			static ShaderLibrary library;
			return library;
		}

		shared_ptr<OpenGLShaderProgram> Load(const std::string& name);
		void Add(const shared_ptr<OpenGLShaderProgram>& shader_program);
		void Remove(const std::string& name);

		bool Exist(const std::string& name);

	private:
		ShaderLibrary() {};
	};
}