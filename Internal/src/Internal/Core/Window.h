/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

#include <stdint.h>
#include "Module.h"
#include <vulkan/vulkan.h>

#define CreateWindow CreateWindow


namespace Internal
{
    struct WindowData
    {
        const char* Title;
        uint32_t width, height;
        bool IsFocused;
    };

    // Base Window Class
    class Window : public Module
    {
    protected:
        Window(const WindowData& data);
    public:
        virtual void setTitle(const char* title) = 0;
        const char* getTitle() const {return m_Data.Title;}

        virtual void setWidth(uint32_t width) = 0;
        virtual void setHeight(uint32_t height) = 0;
        uint32_t getWidth() {return m_Data.width;}
        uint32_t getHeight() {return m_Data.height;}

        bool getFocused() { return m_Data.IsFocused; }

        static bool ShouldClose() { return s_ShouldClose; }

        virtual void OnUpdate() = 0;
        virtual void OnTick() = 0;
        virtual void CreateSurface(VkInstance& instance, VkSurfaceKHR* surface) = 0;

        static Window* CreateWindow(const WindowData& data);
    protected:
        WindowData m_Data;
        inline static bool s_ShouldClose = false;
    };
}