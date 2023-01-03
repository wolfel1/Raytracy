#pragma once

#include "helper/Vector3.h"

namespace raytracy {

	class Raytracer {
		public:
		static void Init();

		static void Shutdown();

		private:
		static Color3 ComputeRayColor();
};
}