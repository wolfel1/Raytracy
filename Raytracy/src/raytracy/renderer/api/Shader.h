#pragma once

typedef unsigned int GLenum;

namespace raytracy {
	class ShaderProgram {
	protected:
		std::string name = "";
		uint32_t renderer_id;

	public:
		virtual ~ShaderProgram() = default;

		const std::string& GetName() const { return name; }

		virtual void Bind() const = 0;

		virtual void Unbind() const = 0;

		static shared_ptr<ShaderProgram> CreateFromFile(const std::string& name);
		static shared_ptr<ShaderProgram> CreateFromDirectory(const std::string& directory_name);

	};

	class ShaderLibrary {
	private:
		std::unordered_map<std::string, shared_ptr<ShaderProgram>> shader_programs;

	public:
		static const std::string rootPath;

	public:
		ShaderLibrary(const ShaderLibrary&) = delete;

		static ShaderLibrary& Get() {
			static ShaderLibrary library;
			return library;
		}

		shared_ptr<ShaderProgram> Load(const std::string& name);
		void Add(const shared_ptr<ShaderProgram>& shader_program);
		void Remove(const std::string& name);

		bool Exist(const std::string& name);

	private:
		ShaderLibrary() {};
	};
}