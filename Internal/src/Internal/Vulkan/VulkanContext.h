/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

#include "Internal/Renderer/GraphicsContext.h"
#include "Internal/Core/Logger.h"
#include "Internal/Events/Event.h"
#include "VulkanImGui.h"

#include <vulkan/vulkan.h>
#include <vector>
#include <cstring>
#include <optional>
#include <string>


namespace Internal
{
	struct QueueFamilyIndices
	{
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;


		bool isComplete()
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VulkanContext : public GraphicsContext
	{
	public:
		VulkanContext();
		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void Shutdown() override;
		void OnEvent(Event& e);
        static const VkDevice& GetDevice();
        static const VkPhysicalDevice& GetPhysicalDevice();
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
	private:
		SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice& device);
        std::vector<char> readFile(const std::string& filename);
        VkShaderModule createShaderModule(const std::vector<char>& code);
		static void CheckVkResult(VkResult err) {if(err != VK_SUCCESS) s_Logger.Error("Failed error check");}
        void recreateSwapChain();
		VkInstance m_Instance;
		static Logger s_Logger;
		const std::vector<const char*> m_ValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};
		const std::vector<const char*> m_DeviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};
		bool m_ValidationLayersEnabled;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		VkSurfaceKHR m_Surface;
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_LogicalDevice;
        static VkDevice s_Device;
        static VkPhysicalDevice s_PhysicalDevice;
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;

		VkSwapchainKHR m_SwapChain;
		std::vector<VkImage> m_SwapChainImages;
		VkFormat m_SwapChainImageFormat;
		VkExtent2D m_SwapChainExtent;
		std::vector<VkImageView> m_SwapChainImageViews;

		VkRenderPass m_RenderPass;
		VkPipelineLayout m_PipelineLayout;
		VkPipeline m_GraphicsPipeline;
		std::vector<VkFramebuffer> m_SwapChainFramebuffers;

		VkCommandPool m_CommandPool;
		std::vector<VkCommandBuffer> m_CommandBuffers;

		std::vector<VkSemaphore> m_ImageAvailableSemaphores;
		std::vector<VkSemaphore> m_RenderFinishedSemaphores;
		std::vector<VkFence> m_InFlightFences;
		std::vector<VkFence> m_ImagesInFlight;

		size_t m_CurrentFrame = 0;

		bool m_FramebufferResized = false;
		const int m_MaxFramesInFlight = 2;
        uint32_t imageIndex;

		VkDescriptorPool m_DescriptorPool;
	};
}