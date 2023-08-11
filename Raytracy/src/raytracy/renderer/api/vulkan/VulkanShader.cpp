#include "raytracypch.h"
#include "VulkanShader.h"

#include <filesystem>
#include "VulkanContext.h"
#include "raytracy/event/ApplicationEvent.h"

namespace raytracy {

	static std::vector<char> ReadFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	shared_ptr<VulkanShader> VulkanShader::Create(const std::vector<std::string>& paths) {
		auto shader = make_shared<VulkanShader>(paths);

		ShaderChangedEvent e(shader);
		EventBus::Get().Notify(e);

		return shader;
	}

	
	VulkanShader::VulkanShader(const std::string& name) {
	}
	VulkanShader::VulkanShader(const std::vector<std::string>& paths) {
		auto vertex_shader_code = ReadFile("resources/shaders/basicspirv/basicvert.spv");
		auto fragment_shader_code = ReadFile("resources/shaders/basicspirv/basicfrag.spv");

		shader_modules[VK_SHADER_STAGE_VERTEX_BIT] = CreateShaderModule(vertex_shader_code);
		shader_modules[VK_SHADER_STAGE_FRAGMENT_BIT] = CreateShaderModule(fragment_shader_code);

		VkVertexInputBindingDescription binding_description{};
		binding_description.binding = 0;
		binding_description.stride = 4 * 3 + 4 * 4;
		binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		this->binding_description = binding_description;

		std::vector<VkVertexInputAttributeDescription> attribute_descriptions{};
		attribute_descriptions.resize(2);
		attribute_descriptions[0].binding = 0;
		attribute_descriptions[0].location = 0;
		attribute_descriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attribute_descriptions[0].offset = 0;

		attribute_descriptions[1].binding = 0;
		attribute_descriptions[1].location = 1;
		attribute_descriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attribute_descriptions[1].offset = 4 * 3;

		this->attribute_descriptions = attribute_descriptions;
	}
	VulkanShader::~VulkanShader() {
	}
	VkShaderModule VulkanShader::CreateShaderModule(const std::vector<char>& code) {
		auto current_context = GraphicsContext::GetCurrentContext();
		auto graphics_context = std::dynamic_pointer_cast<VulkanContext>(current_context);
		RTY_ASSERT(graphics_context, "Context is not Vulkan!");

		VkShaderModuleCreateInfo create_info{};
		create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		create_info.codeSize = code.size();
		create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shader_module;
		if (vkCreateShaderModule(graphics_context->GetLogicalDevice(), &create_info, nullptr, &shader_module) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create shader_module!");
		}

		return shader_module;
	}

	void VulkanShader::Bind() const {
	}
	void VulkanShader::Unbind() const {
	}
}