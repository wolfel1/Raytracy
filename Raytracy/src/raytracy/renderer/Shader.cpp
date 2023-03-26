#include "raytracypch.h"
#include "Shader.h"

#include <filesystem>

#include "glad/gl.h"

namespace raytracy {

	const std::string ShaderProgram::rootPath = "resources/shaders/";

	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex" || type == ".vert")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel" || type == ".frag")
			return GL_FRAGMENT_SHADER;

		RTY_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	shared_ptr<ShaderProgram> CreateFromFile(const std::string& name) {
		auto shader_program = ShaderLibrary::Search(name);

		if (shader_program == nullptr) {
			shader_program = make_shared<ShaderProgram>(name);
		}
		return shader_program;
	}

	// All shader files in the directory must have the same name as the directory
	shared_ptr<ShaderProgram> ShaderProgram::CreateFromDirectory(const std::string& directory_name) {
		auto shader_program = ShaderLibrary::Search(directory_name);

		if (shader_program == nullptr) {
			std::string path = rootPath + directory_name;
			std::vector<std::string> filepaths;
			for (const auto& file : std::filesystem::directory_iterator(path)) {
				filepaths.push_back(file.path().string());
			}

			shader_program = make_shared<ShaderProgram>(filepaths);
		}

		return shader_program;
	}

	ShaderProgram::ShaderProgram(const std::string& name) : name(name) {
		RTY_PROFILE_FUNCTION();

		std::string path = rootPath + name + ".glsl";
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

		ShaderLibrary::Add(shared_ptr<ShaderProgram>(this));
	}

	void ShaderProgram::Bind() const {
		glUseProgram(renderer_id);
	}

	void ShaderProgram::Unbind() const {
		glUseProgram(0);
	}

	std::vector<shared_ptr<ShaderProgram>> ShaderLibrary::shader_programs;

	shared_ptr<ShaderProgram> ShaderLibrary::Search(const std::string& name) {
		if (!(shader_programs.empty() || name.empty())) {
			for (auto& shader_program : shader_programs) {
				if (shader_program->GetName() == name) {
					return shader_program;
				}
			}
		}
		return nullptr;
	}

	void ShaderLibrary::Add(const shared_ptr<ShaderProgram> shader_program) {
		shader_programs.push_back(shader_program);
		RTY_RENDERER_TRACE("{0} shader successfully added!", shader_program->GetName());
	}

	void ShaderLibrary::Remove(const std::string& name) {
		for (auto iterator = shader_programs.cbegin(); iterator != shader_programs.cend(); iterator++) {
			if (iterator->get()->GetName() == name) {
				shader_programs.erase(iterator);
				RTY_RENDERER_TRACE("{0} shader successfully removed!", name);
				return;
			}
		}
		RTY_RENDERER_ERROR("Could not find any shader program refered as {0}!", name);
	}
}