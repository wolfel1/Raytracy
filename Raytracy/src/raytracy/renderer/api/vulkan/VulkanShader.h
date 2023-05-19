#pragma once

#include "../Shader.h"

namespace raytracy {
	class VulkanShader : public Shader {

	public:
		VulkanShader(const std::string& name);
		VulkanShader(const std::vector<std::string>& paths);
		~VulkanShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;
	};

	
}