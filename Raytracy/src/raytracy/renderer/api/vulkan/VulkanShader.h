#pragma once


namespace raytracy {
	class VulkanShader {
	private:
		std::string name;

	public:
		VulkanShader(const std::string& name);
		VulkanShader(const std::vector<std::string>& paths);
		~VulkanShader();

		const std::string GetName() { return name; }

		 void Bind() const;
		 void Unbind() const;

		static shared_ptr<VulkanShader> CreateFromFile(const std::string& name);
		static shared_ptr<VulkanShader> CreateFromDirectory(const std::string& directory_name);
	};

	class ShaderLibrary {
	private:
		std::unordered_map<std::string, shared_ptr<VulkanShader>> shader_programs;

	public:
		static const std::string rootPath;

	public:
		ShaderLibrary(const ShaderLibrary&) = delete;

		static ShaderLibrary& Get() {
			static ShaderLibrary library;
			return library;
		}

		shared_ptr<VulkanShader> Load(const std::string& name);
		void Add(const shared_ptr<VulkanShader>& shader_program);
		void Remove(const std::string& name);

		bool Exist(const std::string& name);

	private:
		ShaderLibrary() {};
	};
}