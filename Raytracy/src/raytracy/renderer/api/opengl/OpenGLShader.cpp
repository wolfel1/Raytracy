#include "raytracypch.h"
#include "OpenGLShader.h"

#include <filesystem>

#include "OpenGLRendererAPI.h"
#include "../../Renderer.h"
#include "../../ViewportScene.h"
#include "../Shader.h"

#include "glad/gl.h"
#include "glm/gtc/type_ptr.hpp"

namespace raytracy {

	uint32_t OpenGLShader::uniform_buffer_index = 0;

	static VertexDataType ConvertOpenGLBaseTypeInVertexDataType(int32_t type) {
		switch (type) {
		case GL_BOOL: return VertexDataType::Bool;
		case GL_FLOAT: return VertexDataType::Float;
		case GL_FLOAT_VEC2: return VertexDataType::Float2;
		case GL_FLOAT_VEC3: return VertexDataType::Float3;
		case GL_FLOAT_VEC4: return VertexDataType::Float4;
		case GL_INT: return VertexDataType::Int;
		case GL_INT_VEC2: return VertexDataType::Int2;
		case GL_INT_VEC3: return VertexDataType::Int3;
		case GL_INT_VEC4: return VertexDataType::Int4;
		case GL_FLOAT_MAT3: return VertexDataType::Mat3;
		case GL_FLOAT_MAT4: return VertexDataType::Mat4;
		default:
			break;
		}

		RTY_RENDERER_ERROR("Unknown OpenGL Type!");
		return VertexDataType::None;
	}

	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex" || type == ".vert")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel" || type == ".frag")
			return GL_FRAGMENT_SHADER;
		if (type == "compute" || type == ".comp")
			return GL_COMPUTE_SHADER;

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

	void OpenGLShader::AddUniformBuffer(shared_ptr<OpenGLUniformBuffer> const uniform_buffer) {
		Bind();
		BindBuffer(uniform_buffer->GetName());
		uniform_buffer->SetBlockIndex(uniform_buffer_index);
		uniform_buffer->Link();
		Unbind();
		uniform_buffer_index++;
	}

	void OpenGLShader::SetInt(std::string const& name, uint32_t value) {
		int texLoc = glGetUniformLocation(renderer_id, name.c_str());
		GLCall(glUniform1i(texLoc, value));
	}
	
	void OpenGLShader::SetVec3(std::string const& name, glm::vec3 const& value) {
		int texLoc = glGetUniformLocation(renderer_id, name.c_str());
		GLCall(glUniform3fv(texLoc, 1, glm::value_ptr(value)));
	}

	OpenGLShader::OpenGLShader(const std::string& name) : name(name) {
		RTY_PROFILE_FUNCTION();

		std::string path = ShaderLibrary::rootPath + name + ".glsl";
		std::string source = ShaderLibrary::ReadFile(path);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		CreateCameraUniformBuffer();
		CreateLightUniformBuffer();
	}

	OpenGLShader::OpenGLShader(const std::vector<std::string>& paths) {
		std::unordered_map<GLenum, std::string> shader_sources;

		for (const auto& path : paths) {
			PreProcess(path, shader_sources);
		}

		Compile(shader_sources);

		CreateCameraUniformBuffer();
		CreateLightUniformBuffer();
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(renderer_id);
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

		std::string source = ShaderLibrary::ReadFile(path);
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

	void OpenGLShader::BindBuffer(std::string const& name) {
		auto uniform_block_index = glGetUniformBlockIndex(renderer_id, name.c_str());
		GLCall(glUniformBlockBinding(renderer_id, uniform_block_index, uniform_buffer_index));
	}

	void OpenGLShader::BindBuffer(shared_ptr<OpenGLUniformBuffer> const uniform_buffer) {
		auto uniform_block_index = glGetUniformBlockIndex(renderer_id, uniform_buffer->GetName().c_str());
		GLCall(glUniformBlockBinding(renderer_id, uniform_block_index, uniform_buffer->GetBlockIndex()));
	}

	void OpenGLShader::Bind() const {
		glUseProgram(renderer_id);
	}

	void OpenGLShader::Unbind() const {
		glUseProgram(0);
	}

	BufferLayout const OpenGLShader::GetUniformBufferLayout(UniformBlock const& uniform_block) const {
		Bind();
		uint32_t ubo_index{};
		int32_t ubo_size;

		ubo_index = glGetUniformBlockIndex(renderer_id, uniform_block.name.c_str());
		RTY_ASSERT(ubo_index != GL_INVALID_INDEX, "Shader must implement uniform block named '{0}'!", uniform_block.name);

		glGetActiveUniformBlockiv(renderer_id, ubo_index, GL_UNIFORM_BLOCK_DATA_SIZE, &ubo_size);

		GLsizei size = static_cast<GLsizei>(uniform_block.uniform_names.size());
		std::vector<GLuint> indices(size);
		std::vector<int32_t> offsets(size);
		std::vector<int32_t> types(size);

		glGetUniformIndices(renderer_id, size, uniform_block.uniform_names.data(), indices.data());
		glGetActiveUniformsiv(renderer_id, size, indices.data(), GL_UNIFORM_OFFSET, offsets.data());
		glGetActiveUniformsiv(renderer_id, size, indices.data(), GL_UNIFORM_TYPE, types.data());
		Unbind();

		std::vector<BufferElement> elements(size);
		for (size_t i = 0; i < size; i++) {
			elements.emplace_back(uniform_block.uniform_names[i], ConvertOpenGLBaseTypeInVertexDataType(types[i]), static_cast<uint32_t>(offsets[i]));
		}

		BufferLayout layout(static_cast<uint32_t>(ubo_size), elements);

		return layout;
	}

	void OpenGLShader::CreateCameraUniformBuffer() {
		auto ubo_index = glGetUniformBlockIndex(renderer_id, "Camera");
		if (ubo_index == GL_INVALID_INDEX) {
			return;
		}
		auto camera = renderer::Scene::Get()->GetCamera();
		auto camera_uniform_buffer = camera->GetCameraUniformBuffer();
		if (!camera_uniform_buffer) {
			UniformBlock block("Camera", {
				"model_matrix",
				"view_projection_matrix",
				"model_view_projection_matrix",
				"normal_matrix",
				"position"
			});

			auto layout = GetUniformBufferLayout(block);

			camera_uniform_buffer = OpenGLUniformBuffer::Create("Camera", layout);
			AddUniformBuffer(camera_uniform_buffer);
			camera->SetCameraUniformBuffer(camera_uniform_buffer);
		} else {
			Bind();
			uint32_t ubo_index = glGetUniformBlockIndex(renderer_id, "Camera");
			RTY_ASSERT(ubo_index != GL_INVALID_INDEX, "Shader must implement uniform block named 'Camera'!");

			AddUniformBuffer(camera_uniform_buffer);
			Unbind();
		}
	}

	void OpenGLShader::CreateLightUniformBuffer() {
		auto ubo_index = glGetUniformBlockIndex(renderer_id, "DirectionalLight");
		if (ubo_index == GL_INVALID_INDEX) {
			return;
		}
		auto light = renderer::Scene::Get()->GetLight();

		if (light) {
			auto light_uniform_buffer = light->GetUniformBuffer();
			if (!light_uniform_buffer) {
				UniformBlock block("DirectionalLight", {
					"lightColor",
					"direction",
					"strength"
				});

				auto layout = GetUniformBufferLayout(block);

				light_uniform_buffer = OpenGLUniformBuffer::Create("DirectionalLight", layout);
				AddUniformBuffer(light_uniform_buffer);
				light->SetUniformBuffer(light_uniform_buffer);
			} else {
				Bind();
				uint32_t ubo_index = glGetUniformBlockIndex(renderer_id, "DirectionalLight");
				RTY_ASSERT(ubo_index != GL_INVALID_INDEX, "Shader must implement uniform block named 'DirectionalLight'!");

				AddUniformBuffer(light_uniform_buffer);
				Unbind();
			}
		}
	}

}