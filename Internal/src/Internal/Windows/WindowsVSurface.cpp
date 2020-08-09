//
// Created by sfulham on 8/10/20.
//

#include "WindowsVSurface.h"

namespace Internal
{
    void WindowsVSurface::Init(HWND hwnd, HINSTANCE hinstance, VkInstance& instance, VkSurfaceKHR* surface)
    {
        VkWin32SurfaceCreateInfoKHR createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.hwnd = hwnd;
        createInfo.hinstance = hinstance;

        if(vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, surface) != VK_SUCCESS)
        {

        }
    }
}