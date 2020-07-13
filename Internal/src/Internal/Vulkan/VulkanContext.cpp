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
		
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
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
			mCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT /*| VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT*/;
			mCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
			mCreateInfo.pfnUserCallback = debugCallback;
			mCreateInfo.pUserData = nullptr;

			if(CreateDebugUtilsMessengerEXT(m_Instance, &mCreateInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS)
			{
				s_Logger.Error("Failed to setup debug messenger");
			}
		}

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
			QueueFamilyIndices indices;
			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

			int i = 0;
			for(const auto& queueFamily : queueFamilies)
			{
				if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					indices.graphicsFamily = i;
				}

				i++;
			}

			isDeviceSuitable = indices.isComplete();

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

	}

	void VulkanContext::SwapBuffers()
	{

	}

	void VulkanContext::Shutdown()
	{
		if(m_ValidationLayersEnabled)
		{
			DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
		}
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