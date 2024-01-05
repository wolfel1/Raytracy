#pragma once

#include "Buffer.h"

typedef unsigned int GLenum;

namespace raytracy {
	class OpenGLShader;

	class ShaderLibrary {
	private:
		std::unordered_map<std::string, shared_ptr<OpenGLShader>> shaders;

	public:
		static const std::string rootPath;

	public:
		ShaderLibrary(const ShaderLibrary&) = delete;
		~ShaderLibrary() = default;

		static ShaderLibrary& Get() {
			static ShaderLibrary library;
			return library;
		}

		static std::string ReadFile(const std::string& path);

		shared_ptr<OpenGLShader> Load(const std::string& name);
		void Add(const shared_ptr<OpenGLShader> shader_program);
		void Remove(const std::string& name);

		bool Exist(const std::string& name);

		void Shutdown() {
			shaders.clear();
		}

	private:
		ShaderLibrary() {};
	};
}