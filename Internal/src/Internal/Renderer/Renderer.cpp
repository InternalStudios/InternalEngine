/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#include "Renderer.h"

namespace Internal
{

    RendererData Renderer::s_Data {};
    VulkanVertexBuffer Renderer::s_Buffer = VulkanVertexBuffer(s_Data.Vertices, sizeof(s_Data.Vertices));

    void Renderer::Init()
    {
        BufferLayout layout = {
                {ShaderDataType::Float2, "inPosition"},
                {ShaderDataType::Float3, "inColor"}
        };
    }

    void Renderer::Draw()
    {

    }

    void Renderer::Shutdown()
    {

    }
}