#pragma once

#include "../Shader.h"
#include <vulkan/vulkan.h>

namespace raytracy {
	class VulkanShader : public Shader {
	private:
		std::unordered_map<VkShaderStageFlagBits, VkShaderModule> shader_modules;
		VkVertexInputBindingDescription binding_description{};
		std::vector<VkVertexInputAttributeDescription> attribute_descriptions{};

	public:
		VulkanShader(const std::string& name);
		VulkanShader(const std::vector<std::string>& paths);
		~VulkanShader();

		static shared_ptr<VulkanShader> Create(const std::vector<std::string>& paths);

		VkVertexInputBindingDescription const GetBindingDescription() { return binding_description; }
		std::vector<VkVertexInputAttributeDescription> const GetAttributeDescriptions() { return attribute_descriptions; }

		const std::unordered_map<VkShaderStageFlagBits, VkShaderModule>& GetShaderModules() const { return shader_modules; }

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetVec4(const std::string& name, const glm::vec4& value) const override {}

		private:
		VkShaderModule CreateShaderModule(const std::vector<char>& code);
	};

	
}