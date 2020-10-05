/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

#include "Internal/Renderer/Buffer.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <inttypes.h>

namespace Internal
{
    class VulkanVertexBuffer : public VertexBuffer
    {
    public:
        VulkanVertexBuffer(float* vertices, uint32_t size);
        ~VulkanVertexBuffer();

        VkPipelineVertexInputStateCreateInfo* GetInfo();

        const VkBuffer* GetBuffer() {return &m_Buffer;}

    private:
        BufferLayout m_Layout;
        VkVertexInputBindingDescription m_Description;
        VkBuffer m_Buffer;
        VkDeviceMemory m_Memory;
    };
}