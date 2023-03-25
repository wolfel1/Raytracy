#pragma once

typedef unsigned int GLenum;

namespace raytracy {
	class ShaderProgram {
	private:
		std::string name = "";
		uint32_t renderer_id;

	public:
		static const std::string rootPath;

		ShaderProgram(const std::string& name);
		ShaderProgram(const std::vector<std::string>& paths);
		~ShaderProgram();

		std::string GetName() const { return name; }

		void Bind() const;

		void Unbind() const;

		static shared_ptr<ShaderProgram> CreateFromFile(const std::string& name);

		static shared_ptr<ShaderProgram> CreateFromDirectory(const std::string& directory_name);

	private:

		std::string ReadFile(const std::string& path);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void PreProcess(const std::string& path, std::unordered_map<GLenum, std::string>& shaderSources);
		void Compile(std::unordered_map<GLenum, std::string> shaderSources);
	};

	class ShaderLibrary {
	private:
		static std::vector<shared_ptr<ShaderProgram>> shader_programs;

	public:
		static shared_ptr<ShaderProgram> Search(const std::string& name);
		static void Add(const shared_ptr<ShaderProgram> shader_program);
		static void Remove(const std::string& name);
	};
}