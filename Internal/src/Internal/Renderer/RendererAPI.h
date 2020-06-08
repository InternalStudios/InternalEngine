/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

namespace Internal
{
    enum class RenderAPI
    {
        None = 0, OpenGL, Metal, Vulkan, DirectX
    };
    class RendererAPI
    {
    public:
        virtual void Init() = 0;
        virtual void DrawIndexed() = 0;
        virtual void ClearColor() = 0;
    private:

    };
}