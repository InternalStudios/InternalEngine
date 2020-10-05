//
// Created by sfulham on 4/10/20.
//

#include "VulkanGraphicsPipeline.h"

#include "VulkanContext.h"

namespace Internal
{
    VulkanGraphicsPipeline::VulkanGraphicsPipeline(const VulkanShader& shader)
    {
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

        if(vkCreatePipelineLayout(VulkanContext::GetDevice(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
        {
            s_Logger.Error("Failed to create pipeline layout");
        }

        std::vector<VkVertexInputAttributeDescription> attributeDescriptions;

        uint32_t offset = 0;
        for(auto& element : shader.GetLayout())
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
            attributeDescriptions.push_back(description);
        }

        VkPipelineVertexInputStateCreateInfo vertexInputInfo {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &m_Description;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(shader.GetLayout().GetElements().size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        VkGraphicsPipelineCreateInfo pipelineInfo {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shader.GetStages();
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

        if(vkCreateGraphicsPipelines(VulkanContext::GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline) != VK_SUCCESS)
        {
            s_Logger.Error("Failed to create pipeline");
        }

        VkCommandPoolCreateInfo poolCreateInfo {};
        poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolCreateInfo.queueFamilyIndex = findQueueFamilies(m_PhysicalDevice, m_Surface).graphicsFamily.value();
        poolCreateInfo.flags = 0;

        if(vkCreateCommandPool(m_LogicalDevice, &poolCreateInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
        {
            s_Logger.Error("Failed to create Command Pool");
        }

        m_CommandBuffers.resize(m_SwapChainFramebuffers.size());

        VkCommandBufferAllocateInfo allocInfo {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_CommandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)m_CommandBuffers.size();

        if(vkAllocateCommandBuffers(m_LogicalDevice, &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
        {
            s_Logger.Error("Failed to allocate command buffers");
        }
    }

    VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
    {
        vkDestroyPipeline(VulkanContext::GetDevice(), m_GraphicsPipeline, nullptr);
        vkDestroyPipelineLayout(VulkanContext::GetDevice(), m_PipelineLayout, nullptr);
    }

    void VulkanGraphicsPipeline::Begin()
    {
        VkCommandBufferBeginInfo beginInfo {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        if(vkBeginCommandBuffer(m_CommandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
        {
            s_Logger.Error("Failed to begin recording command buffer");
        }
        VkRenderPassBeginInfo renderPassBeginInfo {};
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass = m_RenderPass;
        renderPassBeginInfo.framebuffer = m_SwapChainFramebuffers[imageIndex];
        renderPassBeginInfo.renderArea.offset = {0, 0};
        renderPassBeginInfo.renderArea.extent = m_SwapChainExtent;
        VkClearValue clearColor = {1.0f, 0.0f, 1.0f, 1.0f};
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &clearColor;
        vkCmdBeginRenderPass(m_CommandBuffers[imageIndex], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(m_CommandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);
    }

    void VulkanGraphicsPipeline::QueueBuffer(const VulkanVertexBuffer& buffer, const VulkanIndexBuffer& indexBuffer)
    {
        vkCmdBindVertexBuffers(m_CommandBuffers[imageIndex], 0, 1, VulkanVertexBuffer::GetBoundBuffer()->GetBuffer(), 0);
        vkCmdDraw(m_CommandBuffers[imageIndex], 3, 1, 0, 0);
    }

    void VulkanGraphicsPipeline::Submit()
    {
        vkCmdEndRenderPass(m_CommandBuffers[imageIndex]);
        if(vkEndCommandBuffer(m_CommandBuffers[imageIndex]) != VK_SUCCESS)
        {
            s_Logger.Error("Failed to record command buffer");
        }

        VkSubmitInfo submitInfo {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {m_ImageAvailableSemaphores[m_CurrentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_CommandBuffers[imageIndex];
        VkSemaphore signalSemaphores[] = {m_RenderFinishedSemaphores[m_CurrentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(m_LogicalDevice, 1, &m_InFlightFences[m_CurrentFrame]);

        if(vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, m_InFlightFences[m_CurrentFrame]) != VK_SUCCESS)
        {
            s_Logger.Error("Failed to submit draw command buffer");
        }

        VkPresentInfoKHR presentInfo {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        VkSwapchainKHR swapChains[] = {m_SwapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr;
        vkQueuePresentKHR(m_PresentQueue, &presentInfo);
    }
}