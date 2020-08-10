//
// Created by sfulham on 8/10/20.
//

#include "LinuxVSurface.h"

namespace Internal
{
    void LinuxVSurface::Init(::Window window, ::Display* display, VkInstance& instance, VkSurfaceKHR* surface)
    {
        VkXlibSurfaceCreateInfoKHR createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        createInfo.dpy = display;
        createInfo.window = window;
        if(vkCreateXlibSurfaceKHR(instance, &createInfo, nullptr, surface) != VK_SUCCESS)
        {

        }
    }
}