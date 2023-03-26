#include "raytracypch.h"
#include "Shader.h"

#include <filesystem>

#include "glad/gl.h"

namespace raytracy {

	const std::string ShaderLibrary::rootPath = "resources/shaders/";

	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex" || type == ".vert")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel" || type == ".frag")
			return GL_FRAGMENT_SHADER;

		RTY_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	shared_ptr<ShaderProgram> ShaderProgram::CreateFromFile(const std::string& name) {
		return make_shared<ShaderProgram>(name);
	}

	shared_ptr<ShaderProgram> ShaderProgram::CreateFromDirectory(const std::string& directory_name) {
		std::string path = ShaderLibrary::rootPath + directory_name;
		std::vector<std::string> filepaths;
		for (const auto& file : std::filesystem::directory_iterator(path)) {
			filepaths.push_back(file.path().string());
		}

		return make_shared<ShaderProgram>(filepaths);

	}

	ShaderProgram::ShaderProgram(const std::string& name) : name(name) {
		RTY_PROFILE_FUNCTION();

		std::string path = ShaderLibrary::rootPath + name + ".glsl";
		std::string source = ReadFile(path);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);
	}

	ShaderProgram::ShaderProgram(const std::vector<std::string>& paths) {
		std::unordered_map<GLenum, std::string> shader_sources;

		for (const auto path : paths) {
			PreProcess(path, shader_sources);
		}

		Compile(shader_sources);
	}

	ShaderProgram::~ShaderProgram() {
		glDeleteProgram(renderer_id);
	}

	std::string ShaderProgram::ReadFile(const std::string& path) {

		std::string result;
		std::ifstream in(path, std::ios::in | std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1) {
				result.resize(in.tellg());
				in.seekg(0, std::ios::beg);
				in.read(&result[0], result.size());
				in.close();
			} else {
				RTY_RENDERER_ERROR("Could not read from file '{0}'", path);
			}
		} else {
			RTY_RENDERER_ERROR("Could not open file '{0}'", path);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> ShaderProgram::PreProcess(const std::string& source) {

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			RTY_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			RTY_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			RTY_ASSERT(nextLinePos != std::string::npos, "Syntax error");

			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = pos == std::string::npos ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void ShaderProgram::PreProcess(const std::string& path, std::unordered_map<GLenum, std::string>& shaderSources) {

		auto lastDot = path.rfind('.');
		auto count = path.size() - lastDot;
		std::string type = path.substr(lastDot, count);
		RTY_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

		auto lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
		name = path.substr(lastSlash, count);

		std::string source = ReadFile(path);
		shaderSources[ShaderTypeFromString(type)] = source;
	}

	void ShaderProgram::Compile(std::unordered_map<GLenum, std::string> shaderSources) {

		GLuint program = glCreateProgram();
		std::vector<GLenum> glShaderIDs;
		for (auto& kv : shaderSources) {
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE) {
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				RTY_RENDERER_ERROR("{0}", infoLog.data());
				RTY_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs.push_back(shader);
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			for (auto id : glShaderIDs) {
				glDeleteShader(id);
			}

			RTY_RENDERER_ERROR("{0}", infoLog.data());
			RTY_ASSERT(false, "Shader link failure!");
			return;
		}

		for (auto id : glShaderIDs) {
			glDetachShader(program, id);
			glDeleteShader(id);
		}
		renderer_id = program;
	}

	void ShaderProgram::Bind() const {
		glUseProgram(renderer_id);
	}

	void ShaderProgram::Unbind() const {
		glUseProgram(0);
	}


	shared_ptr<ShaderProgram> ShaderLibrary::Load(const std::string& name) {
		std::string path = rootPath + name;
		shared_ptr<ShaderProgram> shader = nullptr;
		if (Exist(name)) {
			return shader_programs[name];
		}
		if (std::filesystem::is_directory(path)) {
			shader = ShaderProgram::CreateFromDirectory(name);
		} else {
			shader = ShaderProgram::CreateFromFile(name);
		}
		RTY_ASSERT(shader, "Could not create shader {0}!", name);
		Add(shader);
		return shader;
	}

	void ShaderLibrary::Add(const shared_ptr<ShaderProgram>& shader_program) {
		shader_programs[shader_program->GetName()] = shader_program;
		RTY_RENDERER_TRACE("{0} shader successfully added!", shader_program->GetName());
	}

	void ShaderLibrary::Remove(const std::string& name) {
		if (Exist(name)) {
			shader_programs.erase(name);
		}
		RTY_RENDERER_ERROR("Could not remove shader program refered as {0}!", name);
	}
	bool ShaderLibrary::Exist(const std::string& name) {
		return shader_programs.find(name) != shader_programs.end();
	}
}