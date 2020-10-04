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

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual const BufferLayout& GetLayout() const override;
        virtual void SetLayout(const BufferLayout& layout);

        static VulkanVertexBuffer* GetBoundBuffer();

        const VkBuffer* GetBuffer() {return &m_Buffer;}

        uint32_t GetSize() const {return m_Size;}
    private:
        BufferLayout m_Layout;
        VkVertexInputBindingDescription m_Description;
        std::vector<VkVertexInputAttributeDescription> m_AttributeDescriptions;
        static VulkanVertexBuffer* s_BoundBuffer;
        VkPipelineVertexInputStateCreateInfo m_Info {};
        VkBuffer m_Buffer;
        VkDeviceMemory m_Memory;
        uint32_t m_Size;
    };
}