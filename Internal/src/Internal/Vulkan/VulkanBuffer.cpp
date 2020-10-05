/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#include "VulkanBuffer.h"
#include "VulkanContext.h"

namespace Internal
{
    static uint32_t findMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags& properties)
    {
        VkPhysicalDeviceMemoryProperties memProps;
        vkGetPhysicalDeviceMemoryProperties(VulkanContext::GetPhysicalDevice(), &memProps);
        for(uint32_t i = 0; i < memProps.memoryTypeCount; i++)
        {
            if((typeFilter & (1 << i) && (memProps.memoryTypes[i].propertyFlags & properties == properties)))
            {
                return i;
            }
        }
        return NULL;
    }

    VulkanVertexBuffer::VulkanVertexBuffer(float *vertices, uint32_t size)
    {
        m_Description.binding = 0;
        m_Description.stride = m_Layout.GetStride() * 32;
        m_Description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        VkBufferCreateInfo bufferInfo {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = sizeof(vertices[0]) * m_Layout.GetElements().size();
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if(vkCreateBuffer(VulkanContext::GetDevice(), &bufferInfo, nullptr, &m_Buffer) != VK_SUCCESS)
        {

        }

        VkMemoryRequirements memReq;
        vkGetBufferMemoryRequirements(VulkanContext::GetDevice(), m_Buffer, &memReq);

        VkMemoryAllocateInfo allocInfo {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memReq.size;
        allocInfo.memoryTypeIndex = findMemoryType(memReq.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        if(vkAllocateMemory(VulkanContext::GetDevice(), &allocInfo, nullptr, &m_Memory) != VK_SUCCESS)
        {

        }

        vkBindBufferMemory(VulkanContext::GetDevice(), m_Buffer, m_Memory, 0);

        void* data;
        vkMapMemory(VulkanContext::GetDevice(), m_Memory, 0, bufferInfo.size, 0, &data);
        memcpy(data, vertices, (size_t)bufferInfo.size);
        vkUnmapMemory(VulkanContext::GetDevice(), m_Memory);
    };

    VulkanVertexBuffer::~VulkanVertexBuffer()
    {
        vkDestroyBuffer(VulkanContext::GetDevice(), m_Buffer, nullptr);
        vkFreeMemory(VulkanContext::GetDevice(), m_Memory, nullptr);
    }
}