/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

//#include <vulkan/vulkan.hpp>

#include "Internal/Core/Core.h"
#include "Internal/Core/Window.h"

#include "VulkanContext.h"

#include "VulkanImGuiRenderer.h"
#include "VulkanBuffer.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <fstream>
#include "Internal/Core/Application.h"
#include <cstdint>
#ifdef INTERNAL_WINDOWS
#include <windows.h>
#include "Internal/Windows/WindowsWindow.h"
#endif



VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if(func)
	{
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	} else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
	
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) 
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

namespace Internal
{

    VkPhysicalDevice VulkanContext::s_PhysicalDevice;
    VkDevice VulkanContext::s_Device;

    const VkPhysicalDevice& VulkanContext::GetPhysicalDevice()
    {
        return s_PhysicalDevice;
    }

    const VkDevice& VulkanContext::GetDevice()
    {
        return s_Device;
    }

    std::vector<char> VulkanContext::readFile(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if(!file.is_open())
        {
            s_Logger.Error("Failed to open file");
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
    }

    QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR& surface)
    {
        QueueFamilyIndices indices;
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            if(presentSupport)
            {
                indices.presentFamily = i;
            }
            i++;
        }

        return indices;
    }

	SwapChainSupportDetails VulkanContext::querySwapChainSupport(const VkPhysicalDevice& device)
	{
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_Surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, nullptr);
		if(formatCount != 0)
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, nullptr);
		if(presentModeCount != 0)
		{
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for(const auto& availableFormat : availableFormats)
		{
			if(availableFormat.format == VK_FORMAT_R8G8B8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& availableModes)
	{
		for(const auto& mode : availableModes)
		{
			if(mode == VK_PRESENT_MODE_FIFO_KHR)
			{
				return mode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if(capabilities.currentExtent.width != UINT32_MAX)
		{
			return capabilities.currentExtent;
		} else
		{
		    uint32_t width, height;
            Application::Get()->GetWindow()->getSize(&width, &height);
			VkExtent2D actualExtent =
			{
			    width,
			    height
			};

			return actualExtent;
		}
		
	}

	Logger VulkanContext::s_Logger("VulkanContext");

	VulkanContext::VulkanContext()
	{

	}

	void VulkanContext::Init()
	{
		#ifdef INTERNAL_DEBUG
		m_ValidationLayersEnabled = true;
		#else
		m_ValidationLayersEnabled = false;
		#endif
		bool validationLayerSupport = true;
		uint32_t layerCount;

		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for(const char* layerName : m_ValidationLayers)
		{
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers)
			{
				if(strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if(!layerFound) {validationLayerSupport = false;}
		}
		if(m_ValidationLayersEnabled && !validationLayerSupport)
		{
			s_Logger.Error("Validation layers requested but not available");
		}

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Sandbox";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "InternalEngine";
		appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
		appInfo.apiVersion = VK_API_VERSION_1_2;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		uint32_t windowExtensionCount = 2;
		const char* windowExtensions[2];

		windowExtensions[0] = VK_KHR_SURFACE_EXTENSION_NAME;
		#ifdef INTERNAL_LINUX
		windowExtensions[1] = "VK_KHR_xcb_surface";
		#elif defined(INTERNAL_WINDOWS)
		windowExtensions[1] = "VK_KHR_win32_surface";
		#endif 

		std::vector<const char*> extensions(windowExtensions, windowExtensions + windowExtensionCount);

		if(m_ValidationLayersEnabled)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();
		
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo {};
		if(m_ValidationLayersEnabled)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
			createInfo.ppEnabledLayerNames = m_ValidationLayers.data();

			debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT /*| VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT*/;
			debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
			debugCreateInfo.pfnUserCallback = debugCallback;
			debugCreateInfo.pUserData = nullptr;

			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
		} else
		{
			createInfo.enabledLayerCount = 0;

			createInfo.pNext = nullptr;
		}
		

		VkResult result = vkCreateInstance(&createInfo, nullptr, &m_Instance);
		if(result == VK_SUCCESS)
		{
			s_Logger.Info("Instance Created");
		} else
		{
			s_Logger.Error("Vulkan Instance not created");
		}

		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> extensionsB(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionsB.data());

		s_Logger.Info("Available extensions:");
		for(const auto& extension : extensionsB)
		{
			s_Logger.Info(extension.extensionName);
		}
		
		if(m_ValidationLayersEnabled)
		{
			VkDebugUtilsMessengerCreateInfoEXT mCreateInfo = {};
			mCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			mCreateInfo.messageSeverity = /*VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |*/ VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT /*| VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT*/;
			mCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
			mCreateInfo.pfnUserCallback = debugCallback;
			mCreateInfo.pUserData = nullptr;

			if(CreateDebugUtilsMessengerEXT(m_Instance, &mCreateInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS)
			{
				s_Logger.Error("Failed to setup debug messenger");
			}
		}

        Application::Get()->GetWindow()->CreateSurface(m_Instance, &m_Surface);
		
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);

		if(deviceCount == 0)
		{
			s_Logger.Error("Failed to find GPU with Vulkan support");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());


		for(const auto& device : devices)
		{
			bool isDeviceSuitable;
			QueueFamilyIndices indices = findQueueFamilies(device, m_Surface);

			bool extensionsSupported = false;
			uint32_t extenstionCount;
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extenstionCount, nullptr);
			std::vector<VkExtensionProperties> availableExtensions(extenstionCount);
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extenstionCount, availableExtensions.data());

			std::set<std::string> requiredExtensions(m_DeviceExtensions.begin(), m_DeviceExtensions.end());

			for(const auto& extension : availableExtensions)
			{
				requiredExtensions.erase(extension.extensionName);
			}
			extensionsSupported = requiredExtensions.empty();

			bool swapChainAdequate = false;
			if(extensionsSupported)
			{
				SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
				swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
			}

			isDeviceSuitable = indices.isComplete() && extensionsSupported && swapChainAdequate;

			if(isDeviceSuitable)
			{
				m_PhysicalDevice = device;
				break;
			}
		}

		if(m_PhysicalDevice == VK_NULL_HANDLE)
		{
			s_Logger.Error("Failed to find a suitable GPU");
		}

        s_PhysicalDevice = m_PhysicalDevice;

		QueueFamilyIndices inds = findQueueFamilies(m_PhysicalDevice, m_Surface);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = {inds.graphicsFamily.value(), inds.presentFamily.value()};
        float queuePriority = 1.0f;

        for(uint32_t queueFamily : uniqueQueueFamilies) {

            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = inds.graphicsFamily.value();
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }
        VkPhysicalDeviceFeatures deviceFeatures {};

        VkDeviceCreateInfo dCreateInfo {};
        dCreateInfo.sType =  VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        dCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        dCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        dCreateInfo.pEnabledFeatures = &deviceFeatures;
        dCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_DeviceExtensions.size());
		dCreateInfo.ppEnabledExtensionNames = m_DeviceExtensions.data();
        if(m_ValidationLayersEnabled)
        {
            dCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
            dCreateInfo.ppEnabledLayerNames = m_ValidationLayers.data();
        } else
        {
            dCreateInfo.enabledLayerCount = 0;
        }

        if(vkCreateDevice(m_PhysicalDevice, &dCreateInfo, nullptr, &m_LogicalDevice) != VK_SUCCESS)
        {
            s_Logger.Error("Failed to create logical device");
        }


        vkGetDeviceQueue(m_LogicalDevice, inds.graphicsFamily.value(), 0, &m_GraphicsQueue);
        vkGetDeviceQueue(m_LogicalDevice, inds.presentFamily.value(), 0, &m_PresentQueue);

        s_Device = m_LogicalDevice;

        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
        pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;
        if(vkCreateDescriptorPool(m_LogicalDevice, &pool_info, nullptr, &m_DescriptorPool) != VK_SUCCESS)
        {
            s_Logger.Error("Failed to create descriptor pool");
        }

		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_PhysicalDevice);
		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = choosePresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
		{
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR scCreateInfo {};
		scCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		scCreateInfo.surface = m_Surface;
		scCreateInfo.minImageCount = imageCount;
		scCreateInfo.imageFormat = surfaceFormat.format;
		scCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
		scCreateInfo.imageExtent = extent;
		scCreateInfo.imageArrayLayers = 1;
		scCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		uint32_t queueFamilyIndices[] = {inds.graphicsFamily.value(), inds.presentFamily.value()};
		if(inds.graphicsFamily != inds.presentFamily)
		{
			scCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			scCreateInfo.queueFamilyIndexCount = 2;
			scCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
		} else {
			scCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			scCreateInfo.queueFamilyIndexCount = 0;
			scCreateInfo.pQueueFamilyIndices = nullptr;
		}

		scCreateInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		scCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		scCreateInfo.presentMode = presentMode;
		scCreateInfo.clipped = VK_TRUE;
		scCreateInfo.oldSwapchain = VK_NULL_HANDLE;

        if(vkCreateSwapchainKHR(m_LogicalDevice, &scCreateInfo, nullptr, &m_SwapChain) != VK_SUCCESS)
        {
            s_Logger.Error("Failed to create Swapchain");
        }

        vkGetSwapchainImagesKHR(m_LogicalDevice, m_SwapChain, &imageCount, nullptr);
        m_SwapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(m_LogicalDevice, m_SwapChain, &imageCount, m_SwapChainImages.data());

        m_SwapChainImageFormat = surfaceFormat.format;
        m_SwapChainExtent = extent;

        m_SwapChainImageViews.resize(m_SwapChainImages.size());
        for(size_t i = 0; i < m_SwapChainImages.size(); i++)
        {
            VkImageViewCreateInfo ivCreateInfo {};
            ivCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            ivCreateInfo.image = m_SwapChainImages[i];
            ivCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            ivCreateInfo.format = m_SwapChainImageFormat;
            ivCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            ivCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            ivCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            ivCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            ivCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            ivCreateInfo.subresourceRange.baseMipLevel = 0;
            ivCreateInfo.subresourceRange.levelCount = 1;
            ivCreateInfo.subresourceRange.baseArrayLayer = 0;
            ivCreateInfo.subresourceRange.layerCount = 1;

            if(vkCreateImageView(m_LogicalDevice, &ivCreateInfo, nullptr, &m_SwapChainImageViews[i]) != VK_SUCCESS)
            {
                s_Logger.Error("Failed to create Image View");
            }
        }

        VkAttachmentDescription colorAttachment {};
        colorAttachment.format = m_SwapChainImageFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef {};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkSubpassDependency dependency {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if(vkCreateRenderPass(m_LogicalDevice, &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
        {
            s_Logger.Error("Failed to create render pass");
        }

        VulkanImGui::Init(m_DescriptorPool, m_LogicalDevice, m_PhysicalDevice, m_Instance, m_SwapChainImages.size(), m_SwapChainImages.size(), m_GraphicsQueue, findQueueFamilies(m_PhysicalDevice, m_Surface).graphicsFamily.value(), m_RenderPass);



        m_SwapChainFramebuffers.resize(m_SwapChainImageViews.size());
        for(size_t i = 0; i < m_SwapChainImageViews.size(); i++)
        {
            VkImageView attachments[] = {
                    m_SwapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = m_RenderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = m_SwapChainExtent.width;
            framebufferInfo.height = m_SwapChainExtent.height;
            framebufferInfo.layers = 1;

            if(vkCreateFramebuffer(m_LogicalDevice, &framebufferInfo, nullptr, &m_SwapChainFramebuffers[i]) != VK_SUCCESS)
            {
                s_Logger.Error("Failed to create framebuffer");
            }
        }



        m_ImageAvailableSemaphores.resize(m_MaxFramesInFlight);
        m_RenderFinishedSemaphores.resize(m_MaxFramesInFlight);
        m_InFlightFences.resize(m_MaxFramesInFlight);
        m_ImagesInFlight.resize(m_SwapChainImages.size(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphoreInfo {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for(size_t i = 0; i <m_MaxFramesInFlight; i++)
        {
            if(vkCreateSemaphore(m_LogicalDevice, &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]) != VK_SUCCESS || vkCreateSemaphore(m_LogicalDevice, &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]) != VK_SUCCESS || vkCreateFence(m_LogicalDevice, &fenceInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS)
            {
                s_Logger.Error("Failed to create semaphores");
            }

        }

        VkCommandBufferAllocateInfo aInfo{};
        aInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        aInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        aInfo.commandPool = m_CommandPool;
        aInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(m_LogicalDevice, &aInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);
        ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(m_GraphicsQueue);

        vkFreeCommandBuffers(m_LogicalDevice, m_CommandPool, 1, &commandBuffer);
	}

	void VulkanContext::SwapBuffers()
	{
	    vkWaitForFences(m_LogicalDevice, 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);

        VkResult result = vkAcquireNextImageKHR(m_LogicalDevice, m_SwapChain, UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, &imageIndex);
        if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        {
            m_FramebufferResized = false;
            return;
        } else if (result != VK_SUCCESS)
        {
            s_Logger.Error("Failed to acquire swap chain images");
        }

        if(m_ImagesInFlight[imageIndex] != VK_NULL_HANDLE)
        {
            vkWaitForFences(m_LogicalDevice, 1, &m_ImagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        }

        m_ImagesInFlight[imageIndex] = m_InFlightFences[m_CurrentFrame];



        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_CommandBuffers[imageIndex]);

        vkQueueWaitIdle(m_PresentQueue);

        m_CurrentFrame = (m_CurrentFrame + 1) % 2;
	}

	void VulkanContext::Shutdown()
	{
	    vkDeviceWaitIdle(m_LogicalDevice);
        vkDestroyDescriptorPool(m_LogicalDevice, m_DescriptorPool, nullptr);
        for(size_t i = 0; i < m_MaxFramesInFlight; i++)
        {
	        vkDestroySemaphore(m_LogicalDevice, m_ImageAvailableSemaphores[i], nullptr);
    	    vkDestroySemaphore(m_LogicalDevice, m_RenderFinishedSemaphores[i], nullptr);
    	    vkDestroyFence(m_LogicalDevice, m_InFlightFences[i], nullptr);
        }
	    vkDestroyCommandPool(m_LogicalDevice, m_CommandPool, nullptr);
	    for(auto framebuffer : m_SwapChainFramebuffers)
        {
	        vkDestroyFramebuffer(m_LogicalDevice, framebuffer, nullptr);
        }
        vkDestroyRenderPass(m_LogicalDevice, m_RenderPass, nullptr);
	    for(auto imageView : m_SwapChainImageViews)
        {
	        vkDestroyImageView(m_LogicalDevice, imageView, nullptr);
        }
	    vkDestroySwapchainKHR(m_LogicalDevice, m_SwapChain, nullptr);
	    vkDestroyDevice(m_LogicalDevice, nullptr);
		if(m_ValidationLayersEnabled)
		{
			DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
		}
		vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
		vkDestroyInstance(m_Instance, nullptr);
	}


	VkBool32 VulkanContext::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		std::stringstream ss;
		ss << "Validation layer: " << pCallbackData->pMessage;
		s_Logger.Output(ss.str().c_str());
		
		return VK_FALSE;
	}

	void VulkanContext::OnEvent(Event& e)
	{
        if(e.GetType() == EventType::WINDOW_RESIZE_EVENT)
        {
        }
	}


}