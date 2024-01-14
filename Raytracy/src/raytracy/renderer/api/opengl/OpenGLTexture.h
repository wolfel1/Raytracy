#pragma once

typedef unsigned int GLenum;

namespace raytracy {
	class OpenGLTexture {

	protected:
		uint32_t renderer_id{};

	public:
		virtual ~OpenGLTexture() = default;

		


		bool operator==(const OpenGLTexture& other) const {
			return renderer_id == ((OpenGLTexture&)other).renderer_id;
		}

	};

	class OpenGLTexture2D : public OpenGLTexture {
	private:

		std::string path;
		uint32_t width{}, height{};
		GLenum internal_format{}, data_format{};

	public:
		OpenGLTexture2D(uint32_t width, uint32_t height, GLenum internal_format);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		void BindImage(uint32_t slot = 0) const;
		void SetData(void* data, uint32_t size);
		void Bind(uint32_t slot = 0) const;
		
		uint32_t GetWidth() const {
			return width;
		}

		uint32_t GetHeight() const {
			return height;
		}

		static shared_ptr<OpenGLTexture2D> Create(uint32_t width, uint32_t height, GLenum internal_format);

		static shared_ptr<OpenGLTexture2D> Create(const std::string& path) {
			return make_shared<OpenGLTexture2D>(path);
		}
	};

	class OpenGLTextureCubemap : public OpenGLTexture {
	private:

	public:
		OpenGLTextureCubemap(const std::vector<std::string>& paths);
		virtual ~OpenGLTextureCubemap();

		void Bind(uint32_t slot = 0) const;

		static shared_ptr<OpenGLTextureCubemap> Create(const std::vector<std::string>& paths) {
			return make_shared<OpenGLTextureCubemap>(paths);
		}
	};
}