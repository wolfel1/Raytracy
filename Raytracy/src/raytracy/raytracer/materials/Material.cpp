#include "raytracypch.h"
#include "Material.h"

namespace raytracy {
	bool LambertianDiffuse::Scatter(const Ray& incoming_ray, const Hit& hit, Color3& attenuation, Ray& scattered) const {
		auto scatter_direction = hit.normal + Random::RandomUnitVector();

		if (glm::all(glm::lessThan(glm::abs(scatter_direction), glm::vec3(1e-8)))) {
			scatter_direction = hit.normal;
		}

		scattered = Ray(hit.point, scatter_direction);
		attenuation = albedo;
		return true;
	}

	bool Metal::Scatter(const Ray& incoming_ray, const Hit& hit, Color3& attenuation, Ray& scattered) const {
		glm::vec3 reflected_ray = incoming_ray.Reflect(hit.normal);

		scattered =
			Ray(hit.point, reflected_ray + Random::RandomVectorInUnitSphere() * fuzzy);
		attenuation = albedo;
		return (glm::dot(scattered.GetDirection(), hit.normal)) > 0;
	}

	bool Dielectric::Scatter(const Ray& incoming_ray, const Hit& hit, Color3& attenuation, Ray& scattered) const {
		attenuation = Color3(1.0f);

		float refraction_ratio =
			hit.front_face ? (1.0f / index_of_refraction) : index_of_refraction;
		float cos_theta = fminf(
			glm::dot(
				-(glm::normalize(incoming_ray.GetDirection())), hit.normal),
			1.0f);
		float sin_theta = sqrt(1.0f - cos_theta * cos_theta);

		bool cannot_refract = refraction_ratio * sin_theta > 1.0f;

		glm::vec3 refracted_direction;
		if (cannot_refract ||
			Reflectance(cos_theta, refraction_ratio) > Random::RandomFloat()) {
			refracted_direction = incoming_ray.Reflect(hit.normal);
		} else {
			refracted_direction = incoming_ray.Refract(hit.normal, refraction_ratio);
		}
		scattered = Ray(hit.point, refracted_direction);
		return true;
	}
	
	float Dielectric::Reflectance(float cosine, float ratio) {
		return 0.0f;
	}
}