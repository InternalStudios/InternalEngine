/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

namespace Internal
{
    enum class GraphicsContexts
    {
        OpenGL, Vulkan, DirectX, Metal
    };

    class GraphicsContext
    {
    public:
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
        virtual void Shutdown() = 0;
    public:
        static GraphicsContexts s_GraphicsContext;
    };
}