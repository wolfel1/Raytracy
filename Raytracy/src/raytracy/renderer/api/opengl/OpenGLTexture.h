#pragma once

#include <glad/gl.h>

namespace raytracy {
	class Texture {

	protected:
		std::string path;
		uint32_t width{}, height{};
		uint32_t renderer_id{};
		GLenum internalFormat{}, dataFormat{};

	public:
		Texture(uint32_t width, uint32_t height) : width(width), height(height) {}
		Texture(const std::string& path) : path(path) {}
		virtual ~Texture() = default;

		uint32_t GetWidth() const {
			return width;
		}

		virtual uint32_t GetHeight() const {
			return height;
		}

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		bool operator==(const Texture& other) const {
			return renderer_id == ((Texture&)other).renderer_id;
		}

	};

	class OpenGLTexture2D : public Texture {

	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();


		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override; 

		static shared_ptr<OpenGLTexture2D> Create(uint32_t width, uint32_t height) {
			return make_shared<OpenGLTexture2D>(width, height);
		}
		static shared_ptr<OpenGLTexture2D> Create(const std::string& path) {
			return make_shared<OpenGLTexture2D>(path);
		}
	};
}