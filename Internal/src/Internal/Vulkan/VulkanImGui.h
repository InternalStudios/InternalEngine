//
// Created by sfulham on 1/10/20.
//

#pragma once

#include <vulkan/vulkan.h>

namespace Internal
{
    class VulkanImGui
    {
    public:
        static void Init(VkDescriptorPool& pool, VkDevice& device, VkPhysicalDevice& pDevice, VkInstance& instance, int ImageCount, int minImageCount, VkQueue& queue, uint32_t queueFamily, VkRenderPass& renderpass);
        static void OnVulkanRender();
        static void Shutdown();
    };
}