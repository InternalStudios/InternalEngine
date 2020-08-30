/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

//#include <vulkan/vulkan.hpp>

#include "Internal/Core/Core.h"

#include "VulkanContext.h"

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

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
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

	SwapChainSupportDetails VulkanContext::querySwapChainSupport(VkPhysicalDevice device)
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

	VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR> availableModes)
	{
		for(const auto& mode : availableModes)
		{
			if(mode == VK_PRESENT_MODE_MAILBOX_KHR)
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
			VkExtent2D actualExtent = {Application::Get()->GetWindow()->getWidth(), Application::Get()->GetWindow()->getHeight()};

			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
		
	}

	VkShaderModule VulkanContext::createShaderModule(const std::vector<char> &code)
	{
        VkShaderModuleCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        if(vkCreateShaderModule(m_LogicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            s_Logger.Error("Failed to create shader");
        }

        return shaderModule;
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
		windowExtensions[1] = "VK_KHR_xlib_surface";
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

        VkRenderPassCreateInfo renderPassInfo {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;

        if(vkCreateRenderPass(m_LogicalDevice, &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
        {
            s_Logger.Error("Failed to create render pass");
        }

        auto vertShaderCode = readFile("assets/shaders/vertex/vertex.spv");
        auto fragShaderCode = readFile("assets/shaders/fragment/fragment.spv");

        VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
        VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

        VkPipelineShaderStageCreateInfo vertShaderStageInfo {};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo {};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        vertShaderStageInfo.module = fragShaderModule;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[2] = {vertShaderStageInfo, fragShaderStageInfo};

        VkPipelineVertexInputStateCreateInfo vertexInputInfo {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexBindingDescriptions = nullptr;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr;

        VkPipelineInputAssemblyStateCreateInfo inputAssembly {};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)m_SwapChainExtent.width;
        viewport.height = (float)m_SwapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor {};
        scissor.offset = {0, 0};
        scissor.extent = m_SwapChainExtent;

        VkPipelineViewportStateCreateInfo viewportState {};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer {};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f;
        rasterizer.depthBiasClamp = 0.0f;
        rasterizer.depthBiasSlopeFactor = 0.0f;

        VkPipelineMultisampleStateCreateInfo multisampling {};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f;
        multisampling.pSampleMask = nullptr;
        multisampling.alphaToCoverageEnable = VK_FALSE;
        multisampling.alphaToOneEnable = VK_FALSE;

        VkPipelineColorBlendAttachmentState colorBlendAttachment {};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        VkPipelineColorBlendStateCreateInfo colorBlending {};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;

        VkDynamicState dynamicStates[] = {
                VK_DYNAMIC_STATE_VIEWPORT
        };
        VkPipelineDynamicStateCreateInfo dynamicState {};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = 0;
        dynamicState.pDynamicStates = dynamicStates;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

        if(vkCreatePipelineLayout(m_LogicalDevice, &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
        {
            s_Logger.Error("Failed to create pipeline layout");
        }

        VkGraphicsPipelineCreateInfo pipelineInfo {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = nullptr;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = nullptr;
        pipelineInfo.layout = m_PipelineLayout;
        pipelineInfo.renderPass = m_RenderPass;
        pipelineInfo.subpass = 0;

        if(vkCreateGraphicsPipelines(m_LogicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline) != VK_SUCCESS)
        {
            s_Logger.Error("Failed to create pipeline");
        }

        vkDestroyShaderModule(m_LogicalDevice, vertShaderModule, nullptr);
        vkDestroyShaderModule(m_LogicalDevice, fragShaderModule, nullptr);
	}

	void VulkanContext::SwapBuffers()
	{

	}

	void VulkanContext::Shutdown()
	{
	    vkDestroyPipeline(m_LogicalDevice, m_GraphicsPipeline, nullptr);
	    vkDestroyPipelineLayout(m_LogicalDevice, m_PipelineLayout, nullptr);
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

}