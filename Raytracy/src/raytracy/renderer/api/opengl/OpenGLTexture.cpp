#include "raytracypch.h"
#include "OpenGLTexture.h"

#include "OpenGLRendererAPI.h"

#include <glad/gl.h>
#include <stb_image.h>

namespace raytracy {
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, GLenum internal_format) : OpenGLTexture(width, height, internal_format) {

		data_format = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id);
		glTextureStorage2D(renderer_id, 1, internal_format, this->width, this->height);

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

}