#pragma once

#include "../GraphicsContext.h"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include "raytracy/core/Window.h"

namespace raytracy {
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT severity,
		VkDebugUtilsMessageTypeFlagsEXT type,
		const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
		void* user_data) {

		std::cerr << "Validation layer: " << callback_data->pMessage << std::endl;

		return VK_FALSE;
	}
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphics_family;
		std::optional<uint32_t> presentation_family;

		bool IsComplete() {
			return graphics_family.has_value() && presentation_family.has_value();
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities{};
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentation_modes;
	};


	class VulkanContext : public GraphicsContext {


	private:
		VkInstance instance{};
		VkDebugUtilsMessengerEXT debug_messenger{};
		VkSurfaceKHR surface{};
		VkQueue presentation_queue{};
		VkPhysicalDevice physical_device = VK_NULL_HANDLE;
		VkDevice logical_device{};
		VkQueue graphics_queue{};

		VkSwapchainKHR swap_chain{};
		std::vector<VkImage> swap_chain_images;
		VkFormat swap_chain_image_format{};
		VkExtent2D swap_chain_extent{};
		std::vector<VkImageView> swap_chain_image_views;
		std::vector<VkFramebuffer> swap_chain_framebuffers;

		VkRenderPass render_pass;

		const std::vector<const char*> validation_layers = {
			"VK_LAYER_KHRONOS_validation"
		};
		const std::vector<const char*> device_extensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

#ifdef RTY_DEBUG
		const bool enable_validation_layers = true;
#else
		const bool enable_validation_layers = false;
#endif
	public:
		VulkanContext() = default;

		const VkExtent2D& GetSwapChainExtent() { return swap_chain_extent; }
		const std::vector<VkFramebuffer>& GetSwapChainFrameBuffers() { return swap_chain_framebuffers; }

		const VkPhysicalDevice& GetPhysicalDevice() { return physical_device; }
		const VkDevice& GetLogicalDevice() { return logical_device; }

		const VkRenderPass& GetRenderPass() { return render_pass; }
		const VkQueue& GetGraphicsQueue() { return graphics_queue; }
		const VkQueue& GetPresentationQueue() { return presentation_queue; }

		const VkSwapchainKHR& GetSwapchain() { return swap_chain; }

		virtual void Init(void* window_handle) override {
			CreateInstance();
			InitDebugMessenger();
			CreateSurface(window_handle);
			PickPhysicalDevice();
			CreateLogicalDevice();
			CreateSwapChain(window_handle);
			CreateImageViews();
			CreateRenderPass();
			CreateFramebuffers();
		};
		QueueFamilyIndices FindQueueFamilyIndices(VkPhysicalDevice device) {
			QueueFamilyIndices indices;
			uint32_t queue_family_count = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);
			std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

			VkBool32 present_support = false;
			int i = 0;
			while (!indices.IsComplete() && i < queue_families.size()) {
				vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support);
				if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					indices.graphics_family = i;
				}
				if (present_support) {
					indices.presentation_family = i;
				}
				i++;
			}

			return indices;
		}
		void RecreateSwapChain() {
			auto* window_handle = m_window->GetNativeWindow();
			RTY_ASSERT(window_handle, "No window available!");
			int width = 0, height = 0;
			glfwGetFramebufferSize(static_cast<GLFWwindow*>(window_handle), &width, &height);
			while (width == 0 || height == 0) {
				glfwGetFramebufferSize(static_cast<GLFWwindow*>(window_handle), &width, &height);
				glfwWaitEvents();
			}
			vkDeviceWaitIdle(logical_device);

			CleanupSwapChain();
			CreateSwapChain(window_handle);
			CreateImageViews();
			CreateFramebuffers();
		}

		virtual void SwapBuffers() override {};

		virtual void SetVSync(bool enabled) override;

		virtual void Shutdown() override {
			CleanupSwapChain();
			vkDestroyRenderPass(logical_device, render_pass, nullptr);
			vkDestroyDevice(logical_device, nullptr);
			vkDestroySurfaceKHR(instance, surface, nullptr);
			if (enable_validation_layers) {
				DestroyDebugUtilsMessengerEXT(instance, debug_messenger, nullptr);
			}
			vkDestroyInstance(instance, nullptr);
		};

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
		void InitDebugMessenger() {
			if (!enable_validation_layers) return;

			VkDebugUtilsMessengerCreateInfoEXT create_info{};
			PopulateDebugMessengerCreateInfo(create_info);

			if (CreateDebugUtilsMessengerEXT(instance, &create_info, nullptr, &debug_messenger)) {
				throw std::runtime_error("Failed to init debug messenger!");
			}
		}

		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
			auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
			if (func != nullptr) {
				return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
			} else {
				return VK_ERROR_EXTENSION_NOT_PRESENT;
			}
		}

		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
			if (func != nullptr) {
				func(instance, debugMessenger, pAllocator);
			}
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

		void CreateSurface(void* window_handle) {
			if (glfwCreateWindowSurface(instance, static_cast<GLFWwindow*>(window_handle), nullptr, &surface)) {
				throw std::runtime_error("Failed to create window surface!");
			}
		}

		void PickPhysicalDevice() {
			uint32_t device_count = 0;
			vkEnumeratePhysicalDevices(instance, &device_count, nullptr);
			if (device_count == 0) {
				throw std::runtime_error("Failed to find GPUs with Vulkan support!");
			}

			std::vector<VkPhysicalDevice> devices(device_count);
			vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

			for (const auto& device : devices) {
				if (IsDeviceSuitable(device)) {
					physical_device = device;
					break;
				}
			}
			if (physical_device == VK_NULL_HANDLE) {
				throw std::runtime_error("failed to find a suitable GPU!");
			}


		}

		bool IsDeviceSuitable(VkPhysicalDevice device) {
			VkPhysicalDeviceProperties device_properties;
			VkPhysicalDeviceFeatures device_features;
			vkGetPhysicalDeviceProperties(device, &device_properties);
			vkGetPhysicalDeviceFeatures(device, &device_features);

			bool is_suitable =
				device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;

			QueueFamilyIndices indices = FindQueueFamilyIndices(device);

			bool extension_support = CheckDeviceExtensionSupport(device);

			bool swap_chain_adequate = false;
			if (extension_support) {
				SwapChainSupportDetails swap_chain_support = QuerySwapChainSupport(device);
				swap_chain_adequate = !swap_chain_support.formats.empty() && !swap_chain_support.presentation_modes.empty();
			}

			return is_suitable && indices.IsComplete() && extension_support && swap_chain_adequate;
		}

		bool CheckDeviceExtensionSupport(VkPhysicalDevice device) {
			uint32_t extension_count;
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
			std::vector<VkExtensionProperties> available_extensions(extension_count);
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

			std::set<std::string> required_extensions(device_extensions.begin(), device_extensions.end());
			for (const auto& extension : available_extensions) {
				required_extensions.erase(extension.extensionName);
			}

			return required_extensions.empty();
		}



		void CreateLogicalDevice() {
			QueueFamilyIndices indices = FindQueueFamilyIndices(physical_device);

			std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
			std::set<uint32_t> unique_queue_families = { indices.graphics_family.value(), indices.presentation_family.value() };

			float queue_priority = 1.0f;
			for (uint32_t queue_family : unique_queue_families) {
				VkDeviceQueueCreateInfo queue_create_info{};
				queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queue_create_info.queueFamilyIndex = queue_family;
				queue_create_info.queueCount = 1;
				queue_create_info.pQueuePriorities = &queue_priority;
				queue_create_infos.push_back(queue_create_info);
			}

			VkPhysicalDeviceFeatures device_features{};

			VkDeviceCreateInfo create_info{};
			create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			create_info.pQueueCreateInfos = queue_create_infos.data();
			create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
			create_info.pEnabledFeatures = &device_features;
			create_info.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
			create_info.ppEnabledExtensionNames = device_extensions.data();

			if (enable_validation_layers) {
				create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
				create_info.ppEnabledLayerNames = validation_layers.data();
			} else {
				create_info.enabledLayerCount = 0;
			}

			if (vkCreateDevice(physical_device, &create_info, nullptr, &logical_device) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create logical device!");
			}

			vkGetDeviceQueue(logical_device, indices.graphics_family.value(), 0, &graphics_queue);
			vkGetDeviceQueue(logical_device, indices.presentation_family.value(), 0, &presentation_queue);
		}

		void CreateSwapChain(void* window_handle) {
			SwapChainSupportDetails swap_chain_support = QuerySwapChainSupport(physical_device);

			VkSurfaceFormatKHR surface_format = ChooseSwapSurfaceFormat(swap_chain_support.formats);
			swap_chain_image_format = surface_format.format;
			VkPresentModeKHR presentation_mode = ChooseSwapPresentMode(swap_chain_support.presentation_modes);
			VkExtent2D extent = ChooseSwapExtent(swap_chain_support.capabilities, window_handle);
			swap_chain_extent = extent;

			uint32_t image_count = swap_chain_support.capabilities.minImageCount + 1;
			if (swap_chain_support.capabilities.maxImageCount > 0 && image_count > swap_chain_support.capabilities.maxImageCount) {
				image_count = swap_chain_support.capabilities.maxImageCount;
			}

			VkSwapchainCreateInfoKHR create_info{};
			create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			create_info.surface = surface;
			create_info.minImageCount = image_count;
			create_info.imageFormat = surface_format.format;
			create_info.imageColorSpace = surface_format.colorSpace;
			create_info.imageExtent = extent;
			create_info.imageArrayLayers = 1;
			create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			QueueFamilyIndices indices = FindQueueFamilyIndices(physical_device);
			uint32_t queue_family_indices[] = { indices.graphics_family.value(), indices.presentation_family.value() };

			if (indices.graphics_family != indices.presentation_family) {
				create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
				create_info.queueFamilyIndexCount = 2;
				create_info.pQueueFamilyIndices = queue_family_indices;
			} else {
				create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
				create_info.queueFamilyIndexCount = 0;
				create_info.pQueueFamilyIndices = nullptr;
			}

			create_info.preTransform = swap_chain_support.capabilities.currentTransform;
			create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			create_info.presentMode = presentation_mode;
			create_info.clipped = VK_TRUE;
			create_info.oldSwapchain = VK_NULL_HANDLE;

			if (vkCreateSwapchainKHR(logical_device, &create_info, nullptr, &swap_chain) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create swap chain!");
			}

			vkGetSwapchainImagesKHR(logical_device, swap_chain, &image_count, nullptr);
			swap_chain_images.resize(image_count);
			vkGetSwapchainImagesKHR(logical_device, swap_chain, &image_count, swap_chain_images.data());
		}

		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device) {
			SwapChainSupportDetails details;

			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

			uint32_t format_count;
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);
			if (format_count != 0) {
				details.formats.resize(format_count);
				vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, details.formats.data());
			}

			uint32_t presentation_mode_count;
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentation_mode_count, nullptr);
			if (presentation_mode_count != 0) {
				details.presentation_modes.resize(presentation_mode_count);
				vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentation_mode_count, details.presentation_modes.data());
			}

			return details;
		}

		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats) {
			for (const auto& available_format : available_formats) {
				if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
					return available_format;
				}
			}

			return available_formats[0];
		}

		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_presentation_modes) {
			if (m_window && !m_window->IsVSync()) {
				for (const auto& available_present_mode : available_presentation_modes) {
					if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
						return available_present_mode;
					}
				}

			}
			return VK_PRESENT_MODE_FIFO_KHR;
		}

		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilites, void* window_handle) {
			if (capabilites.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
				return capabilites.currentExtent;
			} else {
				int width, height;
				glfwGetFramebufferSize(static_cast<GLFWwindow*>(window_handle), &width, &height);

				VkExtent2D actual_extent = {
					static_cast<uint32_t>(width),
					static_cast<uint32_t>(height)
				};

				actual_extent.width = std::clamp(actual_extent.width, capabilites.minImageExtent.width, capabilites.maxImageExtent.width);
				actual_extent.height = std::clamp(actual_extent.height, capabilites.minImageExtent.height, capabilites.maxImageExtent.height);

				return actual_extent;
			}
		}

		void CreateImageViews() {
			swap_chain_image_views.resize(swap_chain_images.size());

			for (size_t i = 0; i < swap_chain_images.size(); i++) {
				VkImageViewCreateInfo create_info{};
				create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				create_info.image = swap_chain_images[i];
				create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
				create_info.format = swap_chain_image_format;
				create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
				create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
				create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
				create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
				create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				create_info.subresourceRange.baseMipLevel = 0;
				create_info.subresourceRange.levelCount = 1;
				create_info.subresourceRange.baseArrayLayer = 0;
				create_info.subresourceRange.layerCount = 1;

				if (vkCreateImageView(logical_device, &create_info, nullptr, &swap_chain_image_views[i]) != VK_SUCCESS) {
					throw std::runtime_error("Failed to create image view!");
				}
			}
		}
		void CreateRenderPass() {
			VkAttachmentDescription color_attachment{};
			color_attachment.format = swap_chain_image_format;
			color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
			color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			VkAttachmentReference color_attachment_ref{};
			color_attachment_ref.attachment = 0;
			color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			VkSubpassDescription subpass_description{};
			subpass_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass_description.colorAttachmentCount = 1;
			subpass_description.pColorAttachments = &color_attachment_ref;

			VkSubpassDependency dependency{};
			dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			dependency.dstSubpass = 0;
			dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.srcAccessMask = 0;
			dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			VkRenderPassCreateInfo render_pass_info{};
			render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			render_pass_info.attachmentCount = 1;
			render_pass_info.pAttachments = &color_attachment;
			render_pass_info.subpassCount = 1;
			render_pass_info.pSubpasses = &subpass_description;
			render_pass_info.dependencyCount = 1;
			render_pass_info.pDependencies = &dependency;

			if (vkCreateRenderPass(logical_device, &render_pass_info, nullptr, &render_pass) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create render pass!");
			}
		}

		void CreateFramebuffers() {
			swap_chain_framebuffers.resize(swap_chain_image_views.size());

			for (size_t i = 0; i < swap_chain_image_views.size(); i++) {
				VkImageView attachments[] = {
					swap_chain_image_views[i]
				};

				VkFramebufferCreateInfo framebuffer_info{};
				framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
				framebuffer_info.renderPass = render_pass;
				framebuffer_info.attachmentCount = 1;
				framebuffer_info.pAttachments = attachments;
				framebuffer_info.width = swap_chain_extent.width;
				framebuffer_info.height = swap_chain_extent.height;
				framebuffer_info.layers = 1;

				if (vkCreateFramebuffer(logical_device, &framebuffer_info, nullptr, &swap_chain_framebuffers[i]) != VK_SUCCESS) {
					throw std::runtime_error("Failed to create framebuffer!");
				}
			}
		}


		void CleanupSwapChain() {
			for (auto framebuffer : swap_chain_framebuffers) {
				vkDestroyFramebuffer(logical_device, framebuffer, nullptr);
			}

			for (auto image_view : swap_chain_image_views) {
				vkDestroyImageView(logical_device, image_view, nullptr);
			}
			vkDestroySwapchainKHR(logical_device, swap_chain, nullptr);
		}

	};
}