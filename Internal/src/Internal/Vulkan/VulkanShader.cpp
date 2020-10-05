/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/


#include "VulkanShader.h"

#include "VulkanContext.h"

namespace Internal
{
    VulkanShader::VulkanShader(std::string &vertexShader, std::string &fragmentShader)
    {
        auto vertShaderCode = readFile(vertexShader);

        VkShaderModuleCreateInfo vertInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = vertShaderCode.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(vertShaderCode.data());

        if(vkCreateShaderModule(VulkanContext::GetDevice(), &vertInfo, nullptr, &m_VertShader) != VK_SUCCESS)
        {
            s_Logger.Error("Failed to create shader");
        }

        auto fragShaderCode = readFile(fragmentShader);

        VkShaderModuleCreateInfo fragInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = fragShaderCode.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(fragShaderCode.data());

        if(vkCreateShaderModule(VulkanContext::GetDevice(), &fragInfo, nullptr, &m_FragShader) != VK_SUCCESS)
        {
            s_Logger.Error("Failed to create shader");
        }

        VkPipelineShaderStageCreateInfo vertShaderStageInfo {};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = m_VertShader;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo {};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = m_FragShader;
        fragShaderStageInfo.pName = "main";

        m_ShaderStages[2] = {vertShaderStageInfo, fragShaderStageInfo};
    }

    VulkanShader::~VulkanShader()
    {
        vkDestroyShaderModule(VulkanContext::GetDevice(), m_VertShader, nullptr);
        vkDestroyShaderModule(VulkanContext::GetDevice(), m_FragShader, nullptr);
    }
}