#pragma once

typedef unsigned int GLenum;

namespace raytracy {

	//representing a shader program including vertex, fragment (pixel), etc. shader
	class Shader {
	protected:
		std::string name = "";
		uint32_t renderer_id{};

	public:
	
		virtual ~Shader() = default;

		const std::string& GetName() const { return name; }

		virtual void Bind() const = 0;

		virtual void Unbind() const = 0;

		static shared_ptr<Shader> CreateFromFile(const std::string& name);
		static shared_ptr<Shader> CreateFromDirectory(const std::string& directory_name);
		
		protected:
		Shader() = default;
		Shader(const std::string& name) : name(name) {};

	};

	class ShaderLibrary {
	private:
		std::unordered_map<std::string, shared_ptr<Shader>> shader_programs;

	public:
		static const std::string rootPath;

	public:
		ShaderLibrary(const ShaderLibrary&) = delete;

		static ShaderLibrary& Get() {
			static ShaderLibrary library;
			return library;
		}

		shared_ptr<Shader> Load(const std::string& name);
		void Add(const shared_ptr<Shader>& shader_program);
		void Remove(const std::string& name);

		bool Exist(const std::string& name);

	private:
		ShaderLibrary() {};
	};
}