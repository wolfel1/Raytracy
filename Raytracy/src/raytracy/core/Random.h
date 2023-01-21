#pragma once

#include <random>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

namespace raytracy {
	class Random {
	private:
		static std::mt19937 generator;
		static std::uniform_real_distribution<float> distribution;

	public:

		static float RandomFloat() {
			// Returns a random real in [0,1).
			return distribution(generator);
		}

		static float RandomFloat(float min, float max) {
			// Returns a random real in [min,max).
			return min + (max - min) * RandomFloat();
		}

		static glm::vec3 RandomVector() {
			return glm::vec3(RandomFloat(), RandomFloat(), RandomFloat());
		}

		static glm::vec3 RandomVector(float min, float max) {
			return glm::vec3(RandomFloat(min, max), RandomFloat(min, max),
							RandomFloat(min, max));
		}

		static glm::vec3 RandomVectorInUnitSphere() {
			return glm::normalize(RandomVector(-1.0f, 1.0f));
		}

		static glm::vec3 RandomUnitVector() {
			return glm::normalize(RandomVectorInUnitSphere());
		}

		static glm::vec3 RandomVectorInHemisphere(const glm::vec3& normal) {
			auto vector_in_unit_sphere = RandomVectorInUnitSphere();
			if (glm::dot(vector_in_unit_sphere, normal) > 0) {
				return vector_in_unit_sphere;
			} else {
				return -vector_in_unit_sphere;
			}
		}

		static glm::vec3 RandomVectorInUnitDisk() {
			return glm::normalize( glm::vec3(RandomFloat(-1, 1), RandomFloat(-1, 1), 0));
		}
	};

}