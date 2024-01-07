#pragma once

typedef unsigned int GLenum;

namespace raytracy {
	class OpenGLTexture {

	protected:
		std::string path;
		uint32_t width{}, height{};
		uint32_t renderer_id{};
		GLenum internal_format{}, data_format{};

	public:
		OpenGLTexture(uint32_t width, uint32_t height, GLenum internal_format) : width(width), height(height), internal_format(internal_format) {}
		OpenGLTexture(const std::string& path) : path(path) {}
		virtual ~OpenGLTexture() = default;

		uint32_t GetWidth() const {
			return width;
		}

		virtual uint32_t GetHeight() const {
			return height;
		}


		bool operator==(const OpenGLTexture& other) const {
			return renderer_id == ((OpenGLTexture&)other).renderer_id;
		}

	};

	class OpenGLTexture2D : public OpenGLTexture {

	public:
		OpenGLTexture2D(uint32_t width, uint32_t height, GLenum internal_format);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		void BindImage(uint32_t slot = 0) const;
		void SetData(void* data, uint32_t size);
		void Bind(uint32_t slot = 0) const;

		static shared_ptr<OpenGLTexture2D> Create(uint32_t width, uint32_t height, GLenum internal_format);

		static shared_ptr<OpenGLTexture2D> Create(const std::string& path) {
			return make_shared<OpenGLTexture2D>(path);
		}
	};
}