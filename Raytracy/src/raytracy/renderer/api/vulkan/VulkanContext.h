#pragma once

#include "../GraphicsContext.h"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace raytracy {
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT severity,
		VkDebugUtilsMessageTypeFlagsEXT type,
		const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
		void* user_data) {

		std::cerr << "Validation layer: " << callback_data->pMessage << std::endl;

		return VK_FALSE;
	}
	class VulkanContext : public GraphicsContext {

	private:
		VkInstance instance{};
		const std::vector<const char*> validation_layers = {
			"VK_LAYER_KHRONOS_validation"
		};

#ifdef RTY_DEBUG
		const bool enable_validation_layers = true;
#else
		const bool enable_validation_layers = false;
#endif
	public:
		VulkanContext(void* window_handle);

		virtual void Init() override {};
		virtual void SwapBuffers() override {};

		private:
			void CreateInstance() {
				if (enable_validation_layers && CheckValidationLayerSupport()) {
					throw std::runtime_error("Validation layers not available!");
				}

				VkApplicationInfo app_info{};
				app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
				app_info.pApplicationName = "Vulkan playground";
				app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
				app_info.pEngineName = "No Engine";
				app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
				app_info.apiVersion = VK_API_VERSION_1_0;

				VkInstanceCreateInfo create_info{};
				create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
				create_info.pApplicationInfo = &app_info;

				auto glfw_extensions = GetRequiredExtensions();
				create_info.enabledExtensionCount = static_cast<uint32_t>(glfw_extensions.size());
				create_info.ppEnabledExtensionNames = glfw_extensions.data();
				VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
				if (enable_validation_layers) {
					create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
					create_info.ppEnabledLayerNames = validation_layers.data();

					PopulateDebugMessengerCreateInfo(debug_create_info);
					create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debug_create_info;
				} else {
					create_info.enabledLayerCount = 0;

					create_info.pNext = nullptr;
				}

				if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS) {
					throw std::runtime_error("Failed to create instance!");
				}
			}
			bool CheckValidationLayerSupport() {
				uint32_t layer_count;
				vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

				std::vector<VkLayerProperties> available_layers(layer_count);
				vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

				for (const char* layer_name : validation_layers) {
					bool layer_found = false;

					for (const auto& layer_properties : available_layers) {
						if (strcmp(layer_name, layer_properties.layerName) == 0) {
							layer_found = true;
							break;
						}
					}

					if (!layer_found) {
						return false;
					}
				}

				return true;
			}

			std::vector<const char*> GetRequiredExtensions() {
				uint32_t extension_count = 0;
				vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
				std::vector<VkExtensionProperties> extensions(extension_count);
				vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());


				uint32_t glfw_extension_count = 0;
				const char** glfw_extensions_pointer;
				glfw_extensions_pointer = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

				std::vector<const char*> glfw_extensions(glfw_extensions_pointer, glfw_extensions_pointer + glfw_extension_count);

				if (enable_validation_layers) {
					glfw_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
				}

				std::cout << extension_count << " extensions supported.\n";
				for (const auto& extension : extensions) {
					std::cout << '\t' << extension.extensionName << '\n';
				}

				std::cout << glfw_extension_count << " extensions used by GLFW.\n";
				for (const auto& extension : glfw_extensions) {
					std::cout << '\t' << extension << '\n';
				}

				return glfw_extensions;
			}
			void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info) {
				create_info = {};
				create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
				create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
					VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
				create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
					VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
				create_info.pfnUserCallback = DebugCallback;
				create_info.pUserData = nullptr;
			}
	};
}