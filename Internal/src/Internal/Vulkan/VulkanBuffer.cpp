/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#include "VulkanBuffer.h"
#include "VulkanContext.h"

namespace Internal
{
     VulkanVertexBuffer* VulkanVertexBuffer::s_BoundBuffer = nullptr;

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
        m_Info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        m_Info.vertexBindingDescriptionCount = 1;
        m_Info.pVertexBindingDescriptions = &m_Description;
        m_Info.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_Layout.GetElements().size());
        m_Info.pVertexAttributeDescriptions = m_AttributeDescriptions.data();

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

        m_Size = m_AttributeDescriptions.size();
    };

    VulkanVertexBuffer::~VulkanVertexBuffer()
    {
        vkDestroyBuffer(VulkanContext::GetDevice(), m_Buffer, nullptr);
        vkFreeMemory(VulkanContext::GetDevice(), m_Memory, nullptr);
    }

    VkPipelineVertexInputStateCreateInfo* VulkanVertexBuffer::GetInfo()
    {
        return &m_Info;
    }

    void VulkanVertexBuffer::SetLayout(const BufferLayout &layout)
    {
        m_Layout = layout;
        uint32_t offset = 0;
        for(auto& element : m_Layout)
        {
            VkVertexInputAttributeDescription description {};
            description.binding = 0;
            description.location = offset;
            switch(element.Type)
            {
                case ShaderDataType::Float:		description.format = VK_FORMAT_R32_SFLOAT;
                case ShaderDataType::Float2:	description.format = VK_FORMAT_R32G32_SFLOAT;
                case ShaderDataType::Float3:	description.format = VK_FORMAT_R32G32B32_SFLOAT;
                case ShaderDataType::Float4:	description.format = VK_FORMAT_R32G32B32A32_SFLOAT;
                case ShaderDataType::Int:		description.format = VK_FORMAT_R32_SINT;
                case ShaderDataType::Int2:		description.format = VK_FORMAT_R32G32_SINT;
                case ShaderDataType::Int3:		description.format = VK_FORMAT_R32G32B32_SINT;
                case ShaderDataType::Int4:		description.format = VK_FORMAT_R32G32B32A32_SINT;
            }
            description.offset = element.Offset;
            m_AttributeDescriptions.push_back(description);
        }
    }

    void VulkanVertexBuffer::Bind()
    {
        s_BoundBuffer = this;
    }

    void VulkanVertexBuffer::Unbind()
    {
        s_BoundBuffer = nullptr;
    }

    VulkanVertexBuffer* VulkanVertexBuffer::GetBoundBuffer()
    {
        return s_BoundBuffer;
    }

    const BufferLayout& VulkanVertexBuffer::GetLayout() const
    {
        return m_Layout;
    }
}