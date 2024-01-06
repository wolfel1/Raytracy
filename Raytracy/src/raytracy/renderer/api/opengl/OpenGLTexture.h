#pragma once

typedef unsigned int GLenum;

namespace raytracy {
	class OpenGLTexture {

	protected:
		std::string path;
		uint32_t width{}, height{};
		uint32_t renderer_id{};
		GLenum internalFormat{}, dataFormat{};

	public:
		OpenGLTexture(uint32_t width, uint32_t height) : width(width), height(height) {}
		OpenGLTexture(const std::string& path) : path(path) {}
		virtual ~OpenGLTexture() = default;

		uint32_t GetWidth() const {
			return width;
		}

		virtual uint32_t GetHeight() const {
			return height;
		}

		void SetData(void* data, uint32_t size);

		void Bind(uint32_t slot = 0) const;

		bool operator==(const OpenGLTexture& other) const {
			return renderer_id == ((OpenGLTexture&)other).renderer_id;
		}

	};

	class OpenGLTexture2D : public OpenGLTexture {

	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		static shared_ptr<OpenGLTexture2D> Create(uint32_t width, uint32_t height) {
			return make_shared<OpenGLTexture2D>(width, height);
		}
		static shared_ptr<OpenGLTexture2D> Create(const std::string& path) {
			return make_shared<OpenGLTexture2D>(path);
		}
	};

	class OpenGLImageTexture2D : public OpenGLTexture {

	public:
		OpenGLImageTexture2D(uint32_t width, uint32_t height);
		virtual ~OpenGLImageTexture2D() = default;

		static shared_ptr<OpenGLImageTexture2D> Create(uint32_t width, uint32_t height) {
			return make_shared<OpenGLImageTexture2D>(width, height);
		}
	};
}