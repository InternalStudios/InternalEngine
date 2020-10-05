/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#include "Internal/Renderer/Shader.h"
#include <string>
#include "Internal/Renderer/Buffer.h"
#include <vulkan/vulkan.h>

namespace Internal
{
    class VulkanShader : public Shader
    {
    public:
        VulkanShader(std::string& vertexShader, std::string& fragmentShader);
        ~VulkanShader();

        void SetLayout(const BufferLayout& layout) {m_BufferLayout = layout;}
        const BufferLayout& GetLayout() const {return m_BufferLayout;}

        VkPipelineShaderStageCreateInfo* GetStages() const {return m_ShaderStages;}
    private:
        VkShaderModule m_FragShader;
        VkShaderModule m_VertShader;
        BufferLayout m_BufferLayout;
        VkPipelineShaderStageCreateInfo m_ShaderStages[];
    };
}