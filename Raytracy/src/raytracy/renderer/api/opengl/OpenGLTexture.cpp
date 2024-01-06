#include "raytracypch.h"
#include "OpenGLTexture.h"

namespace raytracy {
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) : Texture(width, height) {

		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id);
		glTextureStorage2D(renderer_id, 1, internalFormat, this->width, this->height);

		glTextureParameteri(renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : Texture(path) {
	}
	OpenGLTexture2D::~OpenGLTexture2D() {
	}
	void OpenGLTexture2D::SetData(void* data, uint32_t size) {
	}
	void OpenGLTexture2D::Bind(uint32_t slot) const {
	}
}