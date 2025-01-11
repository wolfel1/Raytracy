#pragma once

#include "../api/opengl/OpenGLShader.h"
#include "../api/opengl/OpenGLTexture.h"
namespace raytracy::renderer {

	class Material {
	protected:
		std::string name = "";
		shared_ptr<OpenGLShader> shader;

	public:
		Material(std::string const& name) : name(name) {}
		virtual ~Material() = default;

		virtual std::string GetName() const {
			return name;
		}

		virtual void Draw() const = 0;
	};

	class MeshMaterial : public Material {
	private:
		glm::vec4 color{ 0.8f, 0.8f, 0.8f, 1.0f };
		glm::vec3 specular{ 1.0f };
		float shininess = 32.0f;
		shared_ptr<OpenGLUniformBuffer> material_uniform_buffer = nullptr;

	public:
		MeshMaterial(std::string const& name);
		virtual ~MeshMaterial() = default;

		glm::vec4 GetColor() const {
			return color;
		}

		void SetColor(glm::vec4 const& color) {
			this->color = color;
			material_uniform_buffer->SetVec4("color", color);
		}

		void SetSpecular(glm::vec3 const& specular) {
			this->specular = specular;
			material_uniform_buffer->SetVec3("specular", specular);
		}

		void SetShininess(float shininess) {
			this->shininess = shininess;
			material_uniform_buffer->SetFloat("shininess", shininess);
		}

		virtual void Draw() const override;
	};

	class MaterialLibrary {
	private:
		std::unordered_map<std::string, shared_ptr<MeshMaterial>> materials;

	public:
		MaterialLibrary(const MaterialLibrary&) = delete;
		~MaterialLibrary() = default;

		static MaterialLibrary& Get() {
			static MaterialLibrary library;
			return library;
		}

		shared_ptr<MeshMaterial> Load(const std::string& name);
		void Add(const shared_ptr<MeshMaterial> material);
		void Remove(const std::string& name);

		bool Exist(const std::string& name);

		void Shutdown() {
			materials.clear();
		}

	private:
		MaterialLibrary() {};
	};

	class WorldMaterial : public Material {
	private:
		shared_ptr<OpenGLTextureCubemap> cube_map;

	public:
		WorldMaterial();
		virtual ~WorldMaterial() = default;


		virtual void Draw() const override;
	};
}