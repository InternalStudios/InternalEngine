/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

#include "Buffer.h"
#include "Internal/Types/Pointers.h"
#include "Internal/Vulkan/VulkanBuffer.h"

namespace Internal
{
    struct RendererData
    {
        float Vertices[3 * 5] = {
                0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
        };
    };

    class Renderer
    {
    public:
        static void Init();
        static void Draw();
        static void Shutdown();

    private:
        static VulkanVertexBuffer s_Buffer;
        static RendererData s_Data;
    };
}