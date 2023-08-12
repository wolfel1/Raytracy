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

		const VkExtent2D& GetSwapChainExtent() const { return swap_chain_extent; }
		const std::vector<VkFramebuffer>& GetSwapChainFrameBuffers() const { return swap_chain_framebuffers; }

		const VkPhysicalDevice& GetPhysicalDevice() const { return physical_device; }
		const VkDevice& GetLogicalDevice() const { return logical_device; }

		const VkRenderPass& GetRenderPass() const { return render_pass; }
		const VkQueue& GetGraphicsQueue() const { return graphics_queue; }
		const VkQueue& GetPresentationQueue() const { return presentation_queue; }

		const VkSwapchainKHR& GetSwapchain() const { return swap_chain; }

		virtual void Init(void* window_handle) override;
		QueueFamilyIndices FindQueueFamilyIndices(VkPhysicalDevice device);
		void RecreateSwapChain();

		virtual void SwapBuffers() override {};

		virtual void SetVSync(bool enabled) override;

		virtual void Shutdown() override;

	private:
		void CreateInstance();

		bool CheckValidationLayerSupport();

		std::vector<const char*> GetRequiredExtensions();

		void InitDebugMessenger();

		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info);

		void CreateSurface(void* window_handle);

		void PickPhysicalDevice();

		bool IsDeviceSuitable(VkPhysicalDevice device);

		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

		void CreateLogicalDevice();

		void CreateSwapChain(void* window_handle);

		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats);

		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_presentation_modes);

		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilites, void* window_handle);

		void CreateImageViews();

		void CreateRenderPass();

		void CreateFramebuffers();

		void CleanupSwapChain();

	};
}