#pragma once

#include <string>

#include "Timestep.h"

namespace raytracy {
	
	class Layer {

	public:
		Layer(const std::string& name = "Layer") : debug_name(name) {}
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnUpdate(Timestep timestep) {}
		virtual void OnUIRender() {}

		inline const std::string& GetName() const {
			return debug_name;
		}
	protected:
		std::string debug_name;
	};
}