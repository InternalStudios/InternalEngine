//
// Created by sfulham on 4/10/20.
//

#pragma once

#include "VulkanShader.h"

#include "VulkanBuffer.h"

namespace Internal
{
    class VulkanGraphicsPipeline
    {
    public:
        VulkanGraphicsPipeline(const VulkanShader& shader);
        ~VulkanGraphicsPipeline();

        void Begin();
        void QueueBuffer(const VulkanVertexBuffer& buffer, const VulkanIndexBuffer& indexBuffer);
        void Submit();
    private:
        VkPipelineLayout m_PipelineLayout;
        VkPipeline m_GraphicsPipeline;
    };
}