#pragma once

#include "Buffer.h"

typedef unsigned int GLenum;

namespace raytracy {
	class OpenGLShader;

	class ShaderLibrary {
	private:
		std::unordered_map<std::string, shared_ptr<OpenGLShader>> shader_programs;

	public:
		static const std::string rootPath;

	public:
		ShaderLibrary(const ShaderLibrary&) = delete;

		static ShaderLibrary& Get() {
			static ShaderLibrary library;
			return library;
		}

		shared_ptr<OpenGLShader> Load(const std::string& name);
		void Add(const shared_ptr<OpenGLShader> shader_program);
		void Remove(const std::string& name);

		bool Exist(const std::string& name);

	private:
		ShaderLibrary() {};
	};
}