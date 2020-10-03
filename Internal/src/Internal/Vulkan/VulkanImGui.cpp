//
// Created by sfulham on 1/10/20.
//

#include "VulkanImGui.h"
#include "VulkanImGuiRenderer.h"

namespace Internal {
    void VulkanImGui::Init(VkDescriptorPool& pool, VkDevice& device, VkPhysicalDevice& pDevice, VkInstance& instance, int ImageCount, int minImageCount, VkQueue& queue, uint32_t queueFamily, VkRenderPass& renderpass)
    {
        ImGui_ImplVulkan_InitInfo initInfo {};
        initInfo.Allocator = nullptr;
        initInfo.CheckVkResultFn = [](VkResult result) {if(result != VK_SUCCESS) {return;}};
        initInfo.Device = device;
        initInfo.PhysicalDevice = pDevice;
        initInfo.Instance = instance;
        initInfo.DescriptorPool = pool;
        initInfo.ImageCount = ImageCount;
        initInfo.MinImageCount = minImageCount;
        initInfo.PipelineCache = VK_NULL_HANDLE;
        initInfo.Queue = queue;
        initInfo.QueueFamily = queueFamily;
        ImGui_ImplVulkan_Init(&initInfo, renderpass);
    }

    void VulkanImGui::OnVulkanRender()
    {

    }

    void VulkanImGui::Shutdown()
    {

    }
}
