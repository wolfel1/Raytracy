#include "raytracypch.h"
#include "OpenGLShader.h"

#include <filesystem>

#include "OpenGLRendererAPI.h"
#include "../../Renderer.h"

#include "glad/gl.h"

namespace raytracy {

	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex" || type == ".vert")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel" || type == ".frag")
			return GL_FRAGMENT_SHADER;

		RTY_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	shared_ptr<OpenGLShader> OpenGLShader::CreateFromFile(const std::string& name) {
		return make_shared<OpenGLShader>(name);
	}

	shared_ptr<OpenGLShader> OpenGLShader::CreateFromDirectory(const std::string& directory_name) {
		std::string path = ShaderLibrary::rootPath + directory_name;
		std::vector<std::string> filepaths;
		for (const auto& file : std::filesystem::directory_iterator(path)) {
			filepaths.push_back(file.path().string());
		}

		return make_shared<OpenGLShader>(filepaths);

	}

	OpenGLShader::OpenGLShader(const std::string& name) : Shader(name) {
		RTY_PROFILE_FUNCTION();

		std::string path = ShaderLibrary::rootPath + name + ".glsl";
		std::string source = ReadFile(path);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);
	}

	OpenGLShader::OpenGLShader(const std::vector<std::string>& paths) {
		std::unordered_map<GLenum, std::string> shader_sources;

		for (const auto& path : paths) {
			PreProcess(path, shader_sources);
		}

		Compile(shader_sources);

		Bind();
		uint32_t ubo_index{};
		ubo_index = glGetUniformBlockIndex(renderer_id, "Camera");
		if (Renderer::Get().HasNoCameraUniformBuffer()) {
			int32_t ubo_size;
			glGetActiveUniformBlockiv(renderer_id, ubo_index, GL_UNIFORM_BLOCK_DATA_SIZE, &ubo_size);

			const char* names[4] = {
				"model_matrix",
				"view_matrix",
				"projection_matrix",
				"normal_matrix"
			};

			GLuint indices[4];
			int32_t offsets[4];

			glGetUniformIndices(renderer_id, 4, names, indices);
			glGetActiveUniformsiv(renderer_id, 4, indices, GL_UNIFORM_OFFSET, offsets);

			BufferLayout layout(static_cast<uint32_t>(ubo_size), {
				{ "model_matrix", VertexDataType::Mat4, static_cast<uint32_t>(offsets[0])},
				{ "view_matrix", VertexDataType::Mat4, static_cast<uint32_t>(offsets[1]) },
				{ "projection_matrix", VertexDataType::Mat4, static_cast<uint32_t>(offsets[2]) },
				{ "normal_matrix", VertexDataType::Mat4, static_cast<uint32_t>(offsets[3]) }
			});

			auto camera_uniform_buffer = UniformBuffer::Create("Camera", layout);
			AddUniformBuffer("Camera", camera_uniform_buffer);
			Renderer::Get().SetCameraUniformBuffer(camera_uniform_buffer);
		} else {
			auto camera_uniform_buffer = Renderer::Get().GetCameraUniformBuffer();
			glBindBufferBase(GL_UNIFORM_BUFFER, ubo_index, camera_uniform_buffer->GetID());
			AddUniformBuffer("Camera", camera_uniform_buffer);

		}
		Unbind();
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(renderer_id);
	}

	std::string OpenGLShader::ReadFile(const std::string& path) {

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

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source) {

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

	void OpenGLShader::PreProcess(const std::string& path, std::unordered_map<GLenum, std::string>& shaderSources) {

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

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string> shaderSources) {

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

	void OpenGLShader::BindBuffer(shared_ptr<UniformBuffer> const uniform_buffer) {
		auto& name = uniform_buffer->GetName();
		auto uniform_block_index = glGetUniformBlockIndex(renderer_id, name.c_str());
		GLCall(glUniformBlockBinding(renderer_id, uniform_block_index, index));
	}

	void OpenGLShader::Bind() const {
		glUseProgram(renderer_id);
	}

	void OpenGLShader::Unbind() const {
		glUseProgram(0);
	}

	BufferLayout const OpenGLShader::GetUniformBufferLayout(const std::string& uniform_block_name) const {
		Bind();
		uint32_t ubo_index{};

		int32_t ubo_size;

		ubo_index = glGetUniformBlockIndex(renderer_id, uniform_block_name.c_str());
		glGetActiveUniformBlockiv(renderer_id, ubo_index, GL_UNIFORM_BLOCK_DATA_SIZE, &ubo_size);

		const char* names[3] = {
			"display_color",
			"light_color",
			"light_position"
		};

		GLuint indices[3];
		int32_t offsets[3];

		glGetUniformIndices(renderer_id, 3, names, indices);
		glGetActiveUniformsiv(renderer_id, 3, indices, GL_UNIFORM_OFFSET, offsets);

		BufferLayout layout(static_cast<uint32_t>(ubo_size), {
			{ "display_color", VertexDataType::Float4, static_cast<uint32_t>(offsets[0])},
			{ "light_color", VertexDataType::Float3, static_cast<uint32_t>(offsets[1]) },
			{ "light_position", VertexDataType::Float3, static_cast<uint32_t>(offsets[2]) }
		});
		Unbind();

		return layout;
	}
}