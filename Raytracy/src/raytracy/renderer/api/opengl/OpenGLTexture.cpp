#include "raytracypch.h"
#include "OpenGLTexture.h"

#include "OpenGLRendererAPI.h"

#include <glad/gl.h>
#include <stb_image.h>

namespace raytracy {

	static const std::string rootPath = "resources/textures/";

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, GLenum internal_format) : width(width), height(height), internal_format(internal_format) {

		data_format = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id);
		glTextureStorage2D(renderer_id, 1, internal_format, this->width, this->height);

		glTextureParameteri(renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : path(path) {
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			data = stbi_load((rootPath + path).c_str(), &width, &height, &channels, 0);
		}
		RTY_ASSERT(data, "Failed to load image!");
		this->width = width;
		this->height = height;

		GLenum internal_format = 0, data_format = 0;
		switch (channels) {
		case 4:
			internal_format = GL_RGBA8;
			data_format = GL_RGBA;
			break;
		case 3:
			internal_format = GL_RGB8;
			data_format = GL_RGB;
			break;
		default:
			break;
		}

		this->internal_format = internal_format;
		this->data_format = data_format;

		RTY_ASSERT(internal_format & data_format, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id);
		glTextureStorage2D(renderer_id, 1, internal_format, this->width, this->height);

		glTextureParameteri(renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(renderer_id, 0, 0, 0, this->width, this->height, data_format, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &renderer_id);
	}

	void OpenGLTexture2D::BindImage(uint32_t slot) const {
		GLCall(glBindImageTexture(slot, renderer_id, 0, GL_FALSE, 0, GL_READ_WRITE, internal_format));
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size) {

		uint32_t bpp = data_format == GL_RGBA ? 4 : 3;
		RTY_ASSERT(size == width * height * bpp, "Data must be an entire Texture!");

		glTextureSubImage2D(renderer_id, 0, 0, 0, width, height, data_format, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const {
		glBindTextureUnit(slot, renderer_id);
	}

	shared_ptr<OpenGLTexture2D> OpenGLTexture2D::Create(uint32_t width, uint32_t height, GLenum internal_format = GL_RGBA16) {
		return make_shared<OpenGLTexture2D>(width, height, internal_format);
	}

	OpenGLTextureCubemap::OpenGLTextureCubemap(const std::vector<std::string>& paths) {


		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &renderer_id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, renderer_id);

		int width, height, channels;
		for (int face = 0; face < 6; face++) {
			stbi_uc* data = nullptr;
			{
				data = stbi_load((rootPath + paths[face]).c_str(), &width, &height, &channels, 0);
			}
			RTY_ASSERT(data, "Failed to load image!");

			GLenum internalFormat = 0, dataFormat = 0;
			switch (channels) {
			case 4:
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
				break;
			case 3:
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
				break;
			default:
				break;
			}

			RTY_ASSERT(internalFormat & dataFormat, "Format not supported!");
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}

		glTextureParameteri(renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(renderer_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(renderer_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(renderer_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(renderer_id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}


	OpenGLTextureCubemap::~OpenGLTextureCubemap() {
		glDeleteTextures(1, &renderer_id);
	}

	void OpenGLTextureCubemap::Bind(uint32_t slot) const {
		glBindTextureUnit(slot, renderer_id);
	}

}