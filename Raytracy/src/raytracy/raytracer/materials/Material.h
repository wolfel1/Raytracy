#pragma once

#include "../Ray.h"
#include "../helper/Hit.h"

namespace raytracy {

	class Material {
	public:
		virtual bool Scatter(const Ray& incoming_ray, const Hit& hit,
							 glm::vec3& attenuation, Ray& scattered) const = 0;
	};

	class LambertianDiffuse : public Material {
	private:
		glm::vec3 albedo;

	public:
		LambertianDiffuse(const glm::vec3& albedo) : albedo(albedo) {}

		virtual bool Scatter(const Ray& incoming_ray, const Hit& hit,
							 glm::vec3& attenuation, Ray& scattered) const override;
	};

	class Metal : public Material {
	private:
		glm::vec3 albedo;
		float fuzzy;

	public:
		Metal(const glm::vec3& albedo, float fuzz)
			: albedo(albedo), fuzzy(fuzz < 1 ? fuzz : 1) {
		}

		virtual bool Scatter(const Ray& incoming_ray, const Hit& hit,
							 glm::vec3& attenuation, Ray& scattered) const override;
	};

	class Dielectric : public Material {
	private:
		float index_of_refraction;

	public:
		Dielectric(float index_of_refraction)
			: index_of_refraction(index_of_refraction) {
		}

		virtual bool Scatter(const Ray& incoming_ray, const Hit& hit,
							 glm::vec3& attenuation, Ray& scattered) const override;

	private:
		float Reflectance(float cosine, float ratio) const {
			auto reflectance = (1.0f - ratio) / (1.0f + ratio);
			reflectance = reflectance * reflectance;
			return reflectance + (1.0f - reflectance) * glm::pow((1.0f - cosine), 5.0f);
		}
	};
}

