#include "raytracypch.h"
#include "OpenGLTexture.h"

#include "OpenGLRendererAPI.h"

#include <glad/gl.h>
#include <stb_image.h>

namespace raytracy {
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) : OpenGLTexture(width, height) {

		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id);
		glTextureStorage2D(renderer_id, 1, internalFormat, this->width, this->height);

		glTextureParameteri(renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : OpenGLTexture(path) {
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		RTY_ASSERT(data, "Failed to load image!");
		this->width = width;
		this->height = height;

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

		this->internalFormat = internalFormat;
		this->dataFormat = dataFormat;

		RTY_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id);
		glTextureStorage2D(renderer_id, 1, internalFormat, this->width, this->height);

		glTextureParameteri(renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(renderer_id, 0, 0, 0, this->width, this->height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &renderer_id);
	}

	void OpenGLTexture::SetData(void* data, uint32_t size) {

		uint32_t bpp = dataFormat == GL_RGBA ? 4 : 3;
		RTY_ASSERT(size == width * height * bpp, "Data must be an entire Texture!");

		glTextureSubImage2D(renderer_id, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture::Bind(uint32_t slot) const {
		glBindTextureUnit(slot, renderer_id);
	}

	OpenGLImageTexture2D::OpenGLImageTexture2D(uint32_t width, uint32_t height) : OpenGLTexture(width, height) {
		internalFormat = GL_RGBA32F;
		dataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id);
		glTextureStorage2D(renderer_id, 1, internalFormat, width, height);
		glTextureParameteri(renderer_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(renderer_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(renderer_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		GLCall(glBindImageTexture(0, renderer_id, 0, GL_FALSE, 0, GL_READ_WRITE, internalFormat));
	}
}