#pragma once

#include "../Ray.h"
#include "../helper/Hit.h"

namespace raytracy {

	class Material {
	public:
		virtual bool Scatter(const Ray& incoming_ray, const Hit& hit,
							 Color3& attenuation, Ray& scattered) const = 0;
	};

	class LambertianDiffuse : public Material {
	private:
		Color3 albedo;

	public:
		LambertianDiffuse(const Color3& albedo) : albedo(albedo) {}

		virtual bool Scatter(const Ray& incoming_ray, const Hit& hit,
							 Color3& attenuation, Ray& scattered) const override;
	};

	class Metal : public Material {
	private:
		Color3 albedo;
		float fuzzy;

	public:
		Metal(const Color3& albedo, float fuzz)
			: albedo(albedo), fuzzy(fuzz < 1 ? fuzz : 1) {
		}

		virtual bool Scatter(const Ray& incoming_ray, const Hit& hit,
							 Color3& attenuation, Ray& scattered) const override;
	};

	class Dielectric : public Material {
	private:
		float index_of_refraction;

	public:
		Dielectric(float index_of_refraction)
			: index_of_refraction(index_of_refraction) {
		}

		virtual bool Scatter(const Ray& incoming_ray, const Hit& hit,
							 Color3& attenuation, Ray& scattered) const override;

	private:
		static float Reflectance(float cosine, float ratio);
	};
}

