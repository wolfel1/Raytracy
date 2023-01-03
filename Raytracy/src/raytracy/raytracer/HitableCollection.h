#pragma once

#include "shapes/Hitable.h"

namespace raytracy {
	class HitableCollection {
	private:
		std::vector<shared_ptr<Hitable>> objects;

	public:
		HitableCollection() = default;
		HitableCollection(shared_ptr<Hitable> object) { Add(object); }

		void Clear() { objects.clear(); }
		void Add(shared_ptr<Hitable> object) { objects.push_back(object); }

		bool HitObjects(const Ray& ray, float min, float max, Hit& hit) const;
	};
}


